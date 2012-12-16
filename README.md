#SvcMFCUI

In preamble, I want to tell you that :
*This project is old and didn't get any improvement or corrections
*it is using Windows MFC. I am not good at this and will never be. But At this time I found MFC an "okay" solution for what I wanted to do. But between you and me... this is a rather boring area. So I used it in the best possible way of my knowledge at this time. I am sure that anybody who is more experienced in Windows controls/UI will scream when looking at some parts... I expect it. But at least : this is helping me when I need to get a quick UI.

## What is SvcMFCUI

SvcMFCUI is a **Windows-only** project, for now. However, the interface (API) to drive the UI is C++ and **platform agnostic**. It could be possible to implement a Linux/Apple version.

This dll is a 'service' that I created to get as fast as possible some *User Interface* on various tests I made.

But as many noticed, introducing a UI in a project can be a real pain : the UI requires various conditions at linkage stage; many libraries; many include files. All of the things that most of the time turn a UI into a nightmare.

When I looked at the first time to Maya (Autodesk DCC app), I found their approach rather interesting : MEL script used to build the layout; very simple commands to handle the whole...
Yes... this is never the most friendly way to work and will never be as good as a good interface builder. But for my needs, the idea was good enough.

What I also noticed in some specific conditions: the UI should never be a stopper for the application to run. In other words, the application should even work without an UI. This is true for many experiments. It is even more interesting when you might need this UI *only* when debugging and never later after you released the product (game...).

Finally, I wanted to decouple the implementation of the UI from the way we instanciate it and use it.
For example, I implemented the same UI in OpenGL, so that it could be used as a transparent overlay on top of 3D... In any case, no matter if I used OpenGL or Windows MFC: the interface and the code written to use this UI had to be the exact same.

Note on the OpenGL part: this project got so outdated that I can't ship it. I will have to use another set of widgets, before releasing it...

## Rules
Here are the rules that SvcMFCUI had to follow:
* *No* linkage with any linrary. Only dynamic loading available
* very few include files (so far, 2 files)
* UI build done through methods
* UI Events do be handled in many flexible ways
* simple controls to easily reflect some parameters without too much work
* UI to directly update straighforward variables (float, bool, int, vectors...)
* UI must work with a Console application (need to provide a message pump)
* UI must work with a typical Win32 Application (message pump already there)
* No need to be sexy ^_^... yes, Sorry but don't expect this UI to make your application good looking, like Qt or any. *this is not the putpose*.

This UI turns out to be useful in many prototypes I develop; it is very convenient to have some buttons, sliders, progress-bar and other things when testing 3D effects.
I hope this could help some other developers, too.
