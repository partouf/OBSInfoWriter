
// Stuff to test without having to plug in

#include "InfoWriter.h"

int main()
{
   InfoWriter Writer;
   Writer.GetSettings()->SetFilename("c:/temp/log.txt");
   Writer.GetSettings()->SetFormat("%d:%02d:%02d");
   
   Writer.MarkStart(imtUnknown);

   Writer.WriteInfo(1);

   for (int i = 0; i < INT_MAX; i++)
   {
      i += 1;
      i -= 1;
   }

   Writer.WriteInfo(1);

   Writer.MarkStop(imtUnknown);


   return 0;
}
