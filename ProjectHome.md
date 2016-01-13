Linux port of indie game "Abbaye des Morts". Originally released by Locomalito to Windows platform in year 2010.

| <a href='http://imageshack.us/photo/my-images/31/abbaye01.jpg/'><img src='http://imageshack.us/a/img31/11/abbaye01.th.jpg' border='0' /></a> | <a href='http://imageshack.us/photo/my-images/845/abbaye02.jpg/'><img src='http://imageshack.us/a/img845/6568/abbaye02.th.jpg' border='0' /></a> | <a href='http://imageshack.us/photo/my-images/9/abb16b.png/'><img src='http://imageshack.us/scaled/thumb/9/abb16b.png' border='0' /></a> |
|:---------------------------------------------------------------------------------------------------------------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------|:-----------------------------------------------------------------------------------------------------------------------------------------|

This port uses C language with SDL libraries. The version 1 is using SDL 1 libraries, version 2 will use SDL 2 libraries.

Currently, this port works in:

  * Linux (32 & 64 bits), including Raspberry Pi
  * OpenPandora
  * CGW Zero
  * Nintendo Wii

Fell free if you want to port this program to another system, it's opensource, take the code and modify !

**News**

08/25/2014 - Version 1.2 released ! Carsten Teibes has ported the code to Nintendo Wii (thanks Carsten!) ! Check out [the download page](http://wiibrew.org/wiki/Abbaye). All changes made by Carsten has integrated on the project, also I made some optimizations to SDL1 branch, this should make the game faster. [Here is download link !](https://drive.google.com/file/d/0B7kvXgaMw2iNcE1XNkFKZGV6RW8/edit?usp=sharing)

05/11/2013 - Version 1.13 released. dmitrysmagin ported this source to [CGW Zero console](http://www.gcw-zero.com/) (thanks Dimitry!) and his patches are added to v1 branch. Now 1.13 version can be compiled to Openpandora, CGW Zero & Linux. Check out [Downloads section](http://code.google.com/p/abbaye-for-linux/downloads/list).

23/08/2013 - Version 2.0 released. Some tweaks on beta and the final version is released. The windowed resolution has upgraded to 768x576. This version requires SDL2 libraries installed on your systems. Check out [Downloads section](http://code.google.com/p/abbaye-for-linux/downloads/list).

23/08/2013 - Version 1.12 is released. This version contains 3 patches submitted by rodolfogr & christian.henz (sorry guys for the late response), to avoid memory leaks and better performance on some places. This version requires SDL1 libraries. Check out [Downloads section](http://code.google.com/p/abbaye-for-linux/downloads/list).

22/08/2013 - Beta version of SDL2 port is available. Check out [Downloads section](http://code.google.com/p/abbaye-for-linux/downloads/list) and help to find bugs ! Remember: you need to install SDL2 libraries on your machine.

15/08/2013 - With the release of SDL2, it's time to starting to write the 2.0 version of the game, using the new features of SDL and (I hope) better C code. Want to help ? Join the project ! The SVN repo was updated with the current version (v1 folder) and development work (trunk folder).

22/05/2013 - Thanks to the work of José Cerrejón, Abbaye des Morts now can run on a Rapsberry Pi ! Check out [his blog](http://misapuntesde.com/post.php?id=162") to see how to patch, compile and run the game on this computer (or download deb package to skip the process). I added the deb package to [Downloads section](http://code.google.com/p/abbaye-for-linux/downloads/list).

18/01/2013 - Version 1.11 released. Thanks to Lars Persson (working in the port of the game to Symbian) the music & fx is optimized, loading all the music files at the start of the game (besides to load the files when they're needed). This would help to improve the performance on slow computers. Source code is in [Download](http://code.google.com/p/abbaye-for-linux/downloads/list) section, packages for distributions will come soon.

14/01/2013 - Version 1.1 released. Two bugs fixed, some performance improvements and some clean of source code. You can download from [downloads section](http://code.google.com/p/abbaye-for-linux/downloads/list). Packages for distributions will come as soon as possible.

06/01/2013 - Beta packages for GPH Wiz and Caanoo [here](http://www.gp32x.com/board/index.php?/topic/63691-labbaye-des-morts-wiz-and-caanoo/)

29/12/2012 - Added support for OpenPandora.

29/12/2012 - Doragasu has added as member of the project. He port the game to OpenPandora and integrated the two versions in the same source code.

28/12/2012 - Version 1.0 is ready ! You can get the source, DEBs for Ubuntu 12.10 and RPMs for Fedora 17 & openSuSE 12.2 in [Downloads section](http://code.google.com/p/abbaye-for-linux/downloads/list). A 16-bits tileset style was added, so you can switch between the two styles (8 or 16 bits) in the game pressing "c" key.

26/12/2012 - Christmas passed away, so time to squash bugs. Check out [Source changes](http://code.google.com/p/abbaye-for-linux/source/list) to see whats happening in this project.

23/12/2012 - DEB package for Ubuntu 12.10 32 & 64 bits are ready in Downloads section.

22/12/2012 - Uploaded new version of source code, that solves crashes in Ubuntu/Debian distributions.

21/12/2012 - RPMs packages are ready for openSuSE distribution (version 12.1 and 12.2, i586 and x86\_64). Go here http://software.opensuse.org/package/abbaye?search_term=abbaye. RPMs for Fedora 17 (i686 and x86\_64) are ready in Downloads section.

20/12/2012 - After two months of work, first beta of the game is released. Go to Downloads section to get the source code file, try the game and report bugs !. RPMs files will be added soon.