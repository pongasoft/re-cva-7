CVA-7 - CV Analyzer for Reason
==============================

This project contains the full source code for the free rack extension CVA-7 for Reason, the music DAW produced by Reason Studios. Check the [CVA-7](https://pongasoft.com/rack-extensions/CVA7.html) website for more details.

Notes
-----

* The source code is released following Reason Studios decision to open up the Rack Extension SDK platform allowing for this kind of project to be made publicly available

* It is mostly meant as an example of a fully functional RE and the code is sparsely documented. I have since used C++ more intensively (in Jamba and other VST plugins) and would probably make very different implementation decisions now, especially being more familiar with "modern" C++.

* Unfortunately, the SDK does not come with a CMakefile making this project tied to a single platform (macOS being the one I am using).

* I am also not including any IDE (XCode or other) related files because of how it ends up being tied to install locations (which could be fixed as well with a CMakefile).

Requirements
------------

* This project builds on macOS (10.13.6)

* This project uses RE SDK version 2.2 (location defined in `jukebox.py` and should be changed according to your system). It has **not** been tested with a more recent version of the SDK (I do know for a fact that the options required by `build45.py` have changed since 2.2).

* This projects depends on [pongasoft/re-common@v1.0.1](https://github.com/pongasoft/re-common/tree/v1.0.1) which needs to be installed locally (location defined in `jukebox.py` to be `../re-common` but can be changed)

# Structure

* `Design` contains the Pixelmator files used to create the graphics

* `GUI2D` is a standard RE SDK folder which contains the images (png format) as well as `device_2D.lua` and `hdgui_2D.lua` files which defines the UI

* `Resources` is a standard RE SDK folder which contains the strings (English only) displayed in the UI

* `src/cpp` contains the C++11 source code for the device

* `info.lua`, `motherboard_def.lua` and `realtime_controller.lua` are standard RE SDK files for defining the device

* `display.lua` is the standard SDK file which contains the lua code for the UI (various custom displays)

* `build-dev.sh` and `build-prod.sh` are the main 2 scripts to build the device for dev and prod respectively. They are simple wrapper which invokes the `build45.py` script with the correct set of options

* `copy-resources.sh` is another wrapper script to only copy the resources without full rebuild (ex: when working on the UI only there is no need to rebuild/repackage the source code)

* `render2D.sh` calls the `RE2DRender` process properly (with workaround to see the output in the console). This is the step that processes the content of `GUI2D` to produce the necessary graphic files for the rack extension.

Implementation notes
--------------------

* Because this device uses SDK 2.2, it uses C++11 (but be aware that it is a limited (by the SDK) version of C++11).

* In order to render the waveform of the CV signal in the UI, the implementation uses 256 properties to pass the necessary information from RT to UI. It uses a `PendingUpdates` class to limit the amount of changes per batch. RT owned strings were introduced in SDK 2.5 in order to handle this kind of use case (which would translate into 1 RT owned string property instead of 256 individual properties) and is most likely a much better design.

* The class `Motherboard` models the `motherboard_def.lua` from a C++ point of view

* The properties use a property manager (`IJBoxPropertyManager`) which automatically processes the `iPropertyDiffs` provided on each batch to update the properties.

* The `Device` class represents the entry point and is the class that is the `privateState` in the C-style `JBox_Export_RenderRealtime` api. It contains 2 copies of the `DeviceState` class: one being the state from the previous batch and one the state for the current batch. At the end of the batch, the current state is copied into the previous state. This allow to see if values have changed and respond appropriately. This design inspired the first iteration of the Jamba framework (VST) but with refinement it then led to having the Property (aka Parameters in VST world) themselves hold the previous and current values which I now believe is a better design. I may retrofit this design in a future implementation...

Building (macOS)
----------------

* Run `render2D.sh` to generate the images and files necessary for the UI. This step generates a `GUI` folder with the results.

```
> ./render2D.sh
Rendering 2D
/Volumes/Vault/Applications/JukeboxSDK_2.2/RE2DRender/RE2DRender.app/Contents/MacOS/RE2DRender /Volumes/Development/github/org.pongasoft/re-cva-7/GUI2D /Volumes/Development/github/org.pongasoft/re-cva-7/GUI
Deleting Intermediate folder
Deleting Output folder

[...]

+ exit 0
---- End of PostProcess log
Copying custom_display background image 'Main_LCD_Background.png'
Copying custom_display background image 'Main_LCD_Scrollbar_Background.png'
Copying custom_display background image 'Display_CV_Note_Portrait_Background.png'
Copying custom_display background image 'Display_CV_Value_Background.png'
Copying custom_display background image 'Display_CV_Value_Background.png'
Copying custom_display background image 'Display_CV_Value_Background.png'
Copying custom_display background image 'Display_CV_Value_As_Background.png'
Copying custom_display background image 'Display_CV_Value_As_Background.png'
Copying custom_display background image 'Display_CV_Note_Landscape_Background.png'
Copying custom_display background image 'Display_CV_Value_Background.png'
Copying custom_display background image 'Display_CV_Value_Background.png'
Copying custom_display background image 'Display_CV_Value_Background.png'
Copying custom_display background image 'Display_CV_Value_As_Background.png'
Copying custom_display background image 'Display_CV_Value_As_Background.png'
```

* Run `build-dev.sh`

```
> ./build-dev.sh
Using /Volumes/Vault/Applications/JukeboxSDK_2.2/SDK/Tools/LLVM
product_id: com.pongasoft.CVA7
RACK_EXTENSION_NAME: CVA7

[...]

Copying resources to installDir: /Users/ypujante/Library/Application Support/Propellerhead Software/RackExtensions_Dev/CVA7
install_name_tool -id com.pongasoft.CVA7 '/Users/ypujante/Library/Application Support/Propellerhead Software/RackExtensions_Dev/CVA7/CVA7.dylib'
Build finished
```

* The previous step automatically installs the newly built RE into the default folder for dev REs. You can now start Reason Recon and load the rack extension. Note that it will **not** appear in normal Reason. You need to start Reason Recon.

Release notes
-------------

#### 1.0.0 - 2016/11/30

* First release.


License
-------

- This project is released under the terms of the [Apache 2.0 license](LICENSE.txt)

- This project uses the Rack extension SDK released under the [Rack Extension License agreement](RE_License.txt)
