#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreGraphics/CGImage.h>

@interface PuzzleSolverBridge : NSObject
    
- (UIImage *) detectBoardIn: (UIImage *) image;

- (NSArray *) solvePuzzle: (UIImage *) image;
    
@end
