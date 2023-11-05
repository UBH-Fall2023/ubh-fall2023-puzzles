import SwiftUI
import AVFoundation

// Global variables... don't judge me it's a hackathon!!
var imageView = UIImageView(image: UIImage())
var puzzleSolverBridge = PuzzleSolverBridge()

class ViewController: UIViewController, AVCaptureVideoDataOutputSampleBufferDelegate {
    private var captureSession: AVCaptureSession = AVCaptureSession()
    private let videoDataOutput = AVCaptureVideoDataOutput()

    private func setupCamera() {
        guard let device = AVCaptureDevice.DiscoverySession(deviceTypes: [.builtInWideAngleCamera, .builtInDualCamera, .builtInTrueDepthCamera], mediaType: .video, position: .back).devices.first else {
            fatalError("No back camera device found, please make sure to run SimpleLaneDetection in an iOS device and not a simulator")
        }
        let cameraInput = try! AVCaptureDeviceInput(device: device)
        self.captureSession.addInput(cameraInput)
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        imageView.bounds = view.bounds
        imageView.center = self.view.center;
        imageView.contentMode = .scaleToFill
        self.view.addSubview(imageView)

        self.setupCamera()
        self.setupVideo()
        self.captureSession.startRunning()
    }

    func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        guard let  imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer) else { return }
        CVPixelBufferLockBaseAddress(imageBuffer, CVPixelBufferLockFlags.readOnly)
        let baseAddress = CVPixelBufferGetBaseAddress(imageBuffer)
        let bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer)
        let width = CVPixelBufferGetWidth(imageBuffer)
        let height = CVPixelBufferGetHeight(imageBuffer)
        let colorSpace = CGColorSpaceCreateDeviceRGB()

        var bitmapInfo: UInt32 = CGBitmapInfo.byteOrder32Little.rawValue
        bitmapInfo |= CGImageAlphaInfo.premultipliedFirst.rawValue & CGBitmapInfo.alphaInfoMask.rawValue

        let context = CGContext(data: baseAddress, width: width, height: height, bitsPerComponent: 8, bytesPerRow: bytesPerRow, space: colorSpace, bitmapInfo: bitmapInfo)
        guard let quartzImage = context?.makeImage() else {
            return
        }

        CVPixelBufferUnlockBaseAddress(imageBuffer, CVPixelBufferLockFlags.readOnly)
        let input = UIImage(cgImage: quartzImage)

        // Call the Objective-C which calls the C++!
        let output = puzzleSolverBridge.detectBoard(in: input)

        DispatchQueue.main.async {
            imageView.image = output
        }
    }

    private func setupVideo() {
        videoDataOutput.videoSettings = [(kCVPixelBufferPixelFormatTypeKey as NSString) : NSNumber(value: kCVPixelFormatType_32BGRA)] as [String : Any]
        videoDataOutput.alwaysDiscardsLateVideoFrames = true
        videoDataOutput.setSampleBufferDelegate(self, queue: DispatchQueue(label: "camera.frame.processing.queue"))
        self.captureSession.addOutput(videoDataOutput)
        guard let connection = self.videoDataOutput.connection(with: AVMediaType.video), connection.isVideoOrientationSupported else {
            return
        }
        connection.videoOrientation = .portrait
    }
}

// This is so stupid but it's required for SwiftUI... D:
struct ViewControllerRepresentable: UIViewControllerRepresentable {
    func makeUIViewController(context: Context) -> ViewController {
        return ViewController()
    }

    func updateUIViewController(_ uiViewController: ViewController, context: Context) {

    }
}

enum AppState {
    case camera
    case solved
}


var solved = ""

struct ContentView: View {
    @State private var appState = AppState.camera

    var body: some View {
        if (appState == AppState.camera) {
            VStack {
                ViewControllerRepresentable()
                Button(action: {
                    let a = puzzleSolverBridge.solvePuzzle(imageView.image)
                    if (a != nil && a!.count == 81) {
                        appState = AppState.solved

                        for i in 0..<a!.count {
                            let v = (a![i] as! NSNumber).intValue
                            if (i == 80) {
                                solved += "\(v)"
                            }
                            else if (i % 9 == 8) {
                                solved += "\(v)\n"
                            }
                            else {
                                solved += "\(v)    "
                            }
                        }
                    }
                }) {
                    Text("Solve")
                        .foregroundColor(.white)
                        .padding()
                }
                .background(RoundedRectangle(cornerRadius: 10.0).fill(Color.blue))
                .padding()
            }
        }
        else {
            VStack {
                Text("Solution!")
                    .font(Font.title)
                Text("\(solved)")
                Button(action: {
                    appState = AppState.camera
                    solved = ""
                }) {
                    Text("Done")
                    .foregroundColor(.white)
                    .padding()
                }
                .background(RoundedRectangle(cornerRadius: 10.0).fill(Color.blue))
                .padding()

            }
        }
    }
}

#Preview {
    ContentView()
}
