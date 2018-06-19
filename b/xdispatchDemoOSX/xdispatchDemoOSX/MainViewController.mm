//
//  MainViewController.m
//  xdispatchDemoOSX
//
//  Created by Away on 2018/6/18.
//  Copyright © 2018 Away. All rights reserved.
//

#import "MainViewController.h"
#import <xdispatch/dispatch.h>
@interface MainViewController ()

@end

@implementation MainViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do view setup here.
}
- (IBAction)click:(id)sender {
    NSLog(@"%@, click", [NSThread currentThread]);
    xdispatch::global_queue().async(${
        NSLog(@"%@, 1111", [NSThread currentThread]);
    });
}

@end
