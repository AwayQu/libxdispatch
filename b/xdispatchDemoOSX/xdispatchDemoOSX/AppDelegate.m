//
//  AppDelegate.m
//  xdispatchDemoOSX
//
//  Created by Away on 2018/6/18.
//  Copyright © 2018 Away. All rights reserved.
//

#import "AppDelegate.h"
#import "MainViewController.h"
@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property (nonatomic, strong) NSViewController *mainVC;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {

    __auto_type mainVC = [[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil];
    
    [self.window.contentView addSubview:mainVC.view];
    
    mainVC.view.frame = self.window.contentView.bounds;
    
    self.mainVC = mainVC;
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
