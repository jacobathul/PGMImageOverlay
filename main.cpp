#include "PGMImageData.h"
#include "Utils.h"

#include <iostream>
#include <fstream>
#include <string>

int main( int argc, char** argv )
{
   std::string mainFileString = "";
   std::string overlayFileString = "";
   std::string overlayOperationFlag ="";
   OverlayOperationType type = OverlayOperationType::all;

   switch ( argc )
   {
       case 4:
       {
           overlayOperationFlag = argv[3];
           auto itr = OVERLAY_FLAG_TO_TYPE.find( overlayOperationFlag );

           if ( itr != OVERLAY_FLAG_TO_TYPE.end() )
           {
               type = itr->second;
           }
           else
           {
               std::cout << "Cannot support given flag";
               return 1;
           }
       }
          
       case 3:
       {
           mainFileString = argv[1];
           overlayFileString = argv[2];

           std::ifstream mainFile( mainFileString, std::ios_base::in | std::ios_base::binary );
           std::ifstream overlayFile( overlayFileString, std::ios_base::in | std::ios_base::binary );

           if ( mainFile.is_open() && overlayFile.is_open() )
           {
               std::ofstream outputFile( "Output.pgm", std::ios_base::out | std::ios_base::binary );

               PGMImageData backgroundImage( mainFile );
               PGMImageData overlayImage( overlayFile );

               backgroundImage.AddOverlay( overlayImage , type );
               backgroundImage.WriteToFile( outputFile );

           }
           else
           {
               std::cout << "Cannot open file";
           }
       }
       break;
       default:
           std::cout << "Cannot support given args";
           break;
   }

    return 1;
}
