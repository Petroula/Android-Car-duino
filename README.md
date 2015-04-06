# ANDROID CAR DUINO

![Alt text](Diagram.png?raw=true "You are now looking at the android app")

# Building

###1. Get the android sdk

Either bundled with your IDE or downloaded from https://developer.android.com/sdk/index.html#Other

See \app\build.gradle for version information (subject to change).

###2. Download and extract the openCV for android sdk:
http://sourceforge.net/projects/opencvlibrary/files/opencv-android/

###3. Download the Android-NDK
http://developer.android.com/tools/sdk/ndk/index.html

##4. Set paths in the local.properties file

* If you are using gradle from command line or terminal, you have to create the file manually.

* If you are using android studio, first import the project, then add the ndk dir to the created file.

The local.properties file should contain the following lines, and must NOT be added to git:
```
sdk.dir= **ANDROID SDK DIR**
ndk.dir= **ANDROID NDK DIR**
opencv.dir= **OPENCV ANDROID SDK DIR**
```
Windows file separators ('\') and drive letter separators (':') must be escaped ('\\\\') .The following is an example:
```
sdk.dir=F\:\\Java\\Android\\android-sdk
ndk.dir=F\:\\Java\\Android\\android-ndk-r10d
opencv.dir = F\:\\OpenCV\\OpenCV-android-sdk
```

##5. If you are not using Android Studio

#### Option1 building from terminal/cmd (Recommended)
Run gradlew, or gradlew.bat (from cmd) if using windows.

It will automatically download and extract gradle for you.

You can then run:
> gradlew build

To build the APK's, they are generated to \app\build\outputs
#### Option2 Import the gradle project to your IDE
* The following topic shows how you can genarate a "insert IDE here" project using gradle:
http://stackoverflow.com/questions/10722773/import-existing-gradle-git-project-into-eclipse-for-example-hibernate-orm
* Alternativly, check this out if you want the gradle toolchain integrated into eclipse:
https://github.com/spring-projects/eclipse-integration-gradle/
