#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <string>
#import <iostream>
#import "Interface.hpp"
#import "Vector.hpp"

class ResourceLoader : public IResourceLoader
{
public:
    std::string getResourcePath() const
    {
        NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
        NSString* customResourcePath  = [bundlePath stringByAppendingString:@"/Resources.bundle"];
        return [customResourcePath UTF8String];
    }
    
    void loadPngImage(const std::string& name, int &width, int&height, void** data, int &dataSize)
    {
        if(m_imageData != NULL)
        {
            CFRelease(m_imageData);
            m_imageData = NULL;
        }
        
        NSString* bundlePath = @"/Resources.bundle/";
        NSString* basePath = [NSString stringWithUTF8String:name.c_str()];
        basePath  = [bundlePath stringByAppendingString:basePath];
//        NSString* basePath = [NSString stringWithUTF8String:name.c_str()];

        //NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        //NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        //UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        UIImage *uiImage = nil;
        
        try
        {
            uiImage = [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:basePath ofType:@"png" inDirectory:nil]];
        }
        catch(...)
        {
            return;
        }
        
        if(uiImage == nil)
        {
            return;
        }
        
        CGImageRef cgImage = uiImage.CGImage;
        width = CGImageGetWidth(cgImage);
        height = CGImageGetHeight(cgImage);
        
        //memory leak. convert this to not copy data.
        m_imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        if(m_imageData == NULL)
        {
            dataSize = 0;
            return;
        }
        
        dataSize = CFDataGetLength(m_imageData);
        *data = (void *)CFDataGetBytePtr(m_imageData);
    }
    
private:
    ivec2 m_imageSize; 
    CFDataRef m_imageData;
    
};

IResourceLoader* CreateResourceLoader()
{
    return new ResourceLoader();
}


