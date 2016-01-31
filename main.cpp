
// Stuff to test without having to plug in

#include "InfoWriter.h"

int main()
{
   InfoWriter Writer;
   Writer.GetSettings()->SetFilename("c:/temp/log.txt");
   Writer.GetSettings()->SetFormat("%d:%02d:%02d");
   
   Writer.MarkStart();

   Writer.WriteInfo();

   for (int i = 0; i < INT_MAX; i++)
   {
      i += 1;
      i -= 1;
   }

   Writer.WriteInfo();

   Writer.MarkStop();


   return 0;
}
