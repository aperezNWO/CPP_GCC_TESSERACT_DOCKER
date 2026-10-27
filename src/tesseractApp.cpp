
//////////////////////////////////////////////////////
// CONFIGURATION
//////////////////////////////////////////////////////
/*

1) TOOL CHAIN : C:\msys64\ucrt64\bin (CAMBIAR PATH=)

// INSTALL

	- vcpkg x64-mingw-static

    - vcpkg integrate install

// VISUAL STUDIO CODE 

    - TERMINAL > RUN > BUILD TASK > (2) Build Tesseract DLL

	-  rename file to "tesseract.dll".

	-  Copy "tesseract.dll" file to __dist folder
	
// COMMAND LINE COMPILE (static)

	// original
	
	g++ -shared -static -static-libgcc -static-libstdc++ -o tesseract.dll tesseractApp.cpp -I"C:/Users/pablo.perez/dev/cpp/_install/vcpkg_ucrt64/installed/x64-mingw-static/include"  -L"C:/Users/pablo.perez/dev/cpp/_install/vcpkg_ucrt64/installed/x64-mingw-static/lib"    -ltesseract55 -lleptonica -lcurl -larchive -ltiff -lwebp -lsharpyuv -lgif -lopenjp2 -lssl -lcrypto -lpng -ljpeg -lz -lbz2 -llzma -llz4 -lzstd -lws2_32 -lbcrypt -lcrypt32 -Wl,--subsystem,windows -m64 

	// with libAlgorithm.a ( ar rcs libAlgorithm.a Algorithm.o TFileManager.o RegExManager.o Sudoku.o Dijkstra.o SortBenchMark.o)

	g++ -std=c++23 -shared -static -static-libgcc -static-libstdc++ -o tesseract.dll tesseractApp.cpp -I"include"  -L"lib" -ltesseract55 -lleptonica -lAlgorithm  -lcurl -larchive -ltiff -lwebp -lsharpyuv -lgif -lopenjp2 -lssl -lcrypto -lpng -ljpeg -lz -lbz2 -llzma -llz4 -lzstd -lws2_32 -lbcrypt -lcrypt32 -Wl,--subsystem,windows -DALGORITHM_EXPORTS -m64 

// MAKE (NOT WORKING) 

   mingw32-make

*/


#include "include/tesseractApp.h"
#include "Algorithm.h"

//
TesseractApp::TesseractApp(): Algorithm(false)
{
     //
     this->ReadConfigFile("tesseract.ini");
}
//
TesseractApp::~TesseractApp()
{
    //
}
//
const char* TesseractApp::GetTesseractOcrOutput()
{
   //
   const char* imagePath = "Input.png";
   static std::string output;
   //
   tesseract::TessBaseAPI ocr;

   //
   if (ocr.Init("tessdata", "eng")) {
       output = "Could not initialize tesseract.";
       return output.c_str();
   }

   //
   Pix* image = pixRead(imagePath);
   if (!image) {
       output = "Could not open image.";
       return output.c_str();
   }

   //
   ocr.SetImage(image);
   char* result = ocr.GetUTF8Text();

   //
   output = result ? result : "No output.";
   delete[] result;
   pixDestroy(&image);
   ocr.End();

   //
   return output.c_str();
}
//
const char* TesseractApp::GetTesseractOcrOutputPath(const char* imagePath)
{
   //
   static std::string output;
   //
   tesseract::TessBaseAPI ocr;

   //
   if (ocr.Init("tessdata", "eng")) {
       output = "Could not initialize tesseract.";
       return output.c_str();
   }

   //
   Pix* image = pixRead(imagePath);
   if (!image) {
       output = "Could not open image.";
       return output.c_str();
   }

   //
   ocr.SetImage(image);
   char* result = ocr.GetUTF8Text();

   //
   output = result ? result : "No output.";
   delete[] result;
   pixDestroy(&image);
   ocr.End();

   //
   return output.c_str();
}
//
const char*  TesseractApp::GetTesseractVersion()
{
    // Get the Tesseract version
    return tesseract::TessBaseAPI::Version();
}
//
const char*  TesseractApp::GetTesseractAppVersion()
{
    //
    return this->configMap["DLL_VERSION"].c_str();
} 
//

    
////////////////////////////////////////////////////////////////
// DLL ENTRY POINTS
////////////////////////////////////////////////////////////////

DLL_EXPORT const char*  GetTesseractOcrOutput() 
{
    //
    std::unique_ptr<TesseractApp> uniquePtr = std::make_unique<TesseractApp>();
    //
    return uniquePtr->GetTesseractOcrOutput();
}

DLL_EXPORT const char*  GetTesseractOcrOutputPath(char* imagePath) 
{
    //
    std::unique_ptr<TesseractApp> uniquePtr = std::make_unique<TesseractApp>();
    //
    return uniquePtr->GetTesseractOcrOutputPath(imagePath);
}
                                                     
DLL_EXPORT const char* GetTesseractVersion() 
{
    //
    std::unique_ptr<TesseractApp> uniquePtr = std::make_unique<TesseractApp>();
    //
    return uniquePtr->GetTesseractVersion();
}


DLL_EXPORT const char* GetTesseractAppVersion()
{
    //
    std::unique_ptr<TesseractApp> uniquePtr = std::make_unique<TesseractApp>();
    //
    return uniquePtr->GetTesseractAppVersion();
}


DLL_EXPORT const char* GetTesseract_CPPSTDVersion()
{
    static std::string version;
    if (version.empty()) {
        TesseractApp app;
        version = app.GetCPPSTDVersion(__cplusplus);
    }
    return version.c_str();
}
