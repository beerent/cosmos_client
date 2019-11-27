//
//  ViewController.h
//  SingleViewApp
//
//  Created by Kamil Konecko on 4/2/16.
//  Copyright © 2016 Kamil Konecko. All rights reserved.
//
#pragma once
#import <UIKit/UIKit.h>
#import "GLView.h"

@interface ViewController : UIViewController {
@private
GLView* m_view;
UIWindow* m_window;
}
@end

