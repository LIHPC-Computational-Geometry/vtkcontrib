// ************************************************************************* //
//                                DebugStream.h                              //
// ************************************************************************* //

#ifndef DEBUG_STREAM_H
#define DEBUG_STREAM_H
#include <VtkContrib/misc_exports.h>

//#include <iostream.h>
//#include <fstream.h>
#include <iostream>		// CP pour SGI
#include <fstream>		// CP pour SGI
#include <vector>
#include <signal.h>
#include <stdlib.h>


// ****************************************************************************
//  Class:  DebugStream
//
//  Purpose:
//    An implementation of ostream designed for error and message logging.
//    A DebugStream has a "level" assigned to it as a priority (lower number
//    is higher priority).  Multiple DebugStreams can be created.  When
//    output is written to one DebugStream, it is simultaneously written to
//    all DebugStreams with a higher level.
//
//    In this specific implementation, five debug levels are used.  Only
//    the highest priority messages go to debug1, but every message will go
//    to debug5.
//
//  Programmer:  Jeremy Meredith
//  Creation:    November 16, 2000
//
//  Modified:
//    Jeremy Meredith, Fri Jun  1 17:18:03 PDT 2001
//    Added the ability to check if a DebugStream was enabled.
//
//    Brad Whitlock, Thu Mar 14 13:37:34 PST 2002
//    Added exports.
//
// ****************************************************************************

class MISC_API DebugStream : public ostream
{
  public:
                   DebugStream(int level_);
                  ~DebugStream();

    virtual        operator bool()               {return enabled;};
    void           open(const char *progname);
    void           close();
    void           SetDeleteOnClose(bool d);
    static void    Initialize(const char *, int);
  private:
//    class DebugStreamBuf : public streambuf
    class DebugStreamBuf : public std::streambuf		// CP pour SGI
    {
      public:
                       DebugStreamBuf();
                      ~DebugStreamBuf();

        void           SetDeleteOnClose(bool d);
        void           SetLevel(int level_);

        void           close();
        void           open(const char *filename_);
        int            put(int c);
        virtual int    overflow(int c);
        virtual int    underflow() { return 0; };
      private:
        // the priority of the current messages being logged
        static int                            curLevel;
        // the list of all buffers
        static std::vector<DebugStreamBuf*>   allBuffers;

        // the level of this DebugStreamBuf
        int                                   level;
        // the filename being to which to write the messages
        char                                  filename[256];
        // whether or not to delete the file when the streambuf is closed
        bool                                  deleteOnClose;
        // the ostream used to write the log file
        std::ofstream                         *out;
    };


    // the streambuf used for this ostream
    DebugStreamBuf   buf;
    // the level of this DebugStream
    int              level;
    // true if enabled
    bool             enabled;
};

extern MISC_API DebugStream debug1;
extern MISC_API DebugStream debug2;
extern MISC_API DebugStream debug3;
extern MISC_API DebugStream debug4;
extern MISC_API DebugStream debug5;

#endif


