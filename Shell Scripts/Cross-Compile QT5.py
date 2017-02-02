from contextlib import contextmanager
import urllib.request
import zipfile
import hashlib
import os
import shutil
import warnings
import fileinput
import subprocess
from os.path import expanduser

# Constant Variable Definitions
QT_SRC_URL = "http://download.qt.io/official_releases/qt/5.8/5.8.0/single/qt-everywhere-opensource-src-5.8.0.zip"
QT_SRC_CHECKSUM = "c57cf81c1394230c5a188b7601bb4c072314cb350d5d3d6b5b820426c60570e5"
QT_SRC_DIR = expanduser("~/qt-everywhere-opensource-src-5.8.0") # Note: Should match directory of qt-everywhere-opensource-src-5.8.0.zip

scriptDir = os.path.dirname(__file__) #Path that the script is in.
stageDir = os.path.join(scriptDir, "temp")
stagePath = os.path.join(stageDir, "CrossCompileQT5.stg")

# Adapted from https://visualgdb.com/tutorials/raspberry/qt/embedded/
@contextmanager
def logStage(stageNum, stageString):
    print("Stage ",stageNum,": ",stageString);
    stageFile = open(stagePath, "w");
    stageFile.truncate()
    stageFile.write(str(stageNum))
    yield
    stageFile.close()
    return


stageNum = 1

print("Getting stage number...")


if(os.path.exists(stageDir)):
    if (os.path.exists(stagePath)):
        stageFile = open(stagePath)
        try:
            stageNum = int(stageFile.read(1))
            print("Found stage number", stageNum)
        except ValueError:
            pass
        stageFile.close()
else:
    os.makedirs(stageDir)
    

if(stageNum == 1):
    with logStage(stageNum, "Download qt-everywhere-opensource-src-5.8.0.zip"):
        with urllib.request.urlopen(QT_SRC_URL) as f, open(expanduser("~/qt-everywhere-opensource-src-5.8.0.zip"), "wb") as outfile:
            shutil.copyfileobj(f, outfile)

        
        # urlretrieve("http://download.qt.io/official_releases/qt/5.8/5.8.0/single/qt-everywhere-opensource-src-5.8.0.zip", expanduser("~/qt-everywhere-opensource-src-5.8.0.zip"))
    stageNum += 1

if(stageNum == 2):
    with logStage(stageNum, "Check sha-256 hash of downloaded file."):
        with open(expanduser("~/qt-everywhere-opensource-src-5.8.0.zip"), "rb") as myFile:
            hash_sha256 = hashlib.sha256()
            for chunk in iter(lambda: myFile.read(4096), b""):
                hash_sha256.update(chunk)
            if (hash_sha256.hexdigest() != QT_SRC_CHECKSUM):
                raise RuntimeError("SHA-256 hash for qt-everywhere-opensource-src-5.8.0.zip does not match expected value.")
            else:
                stageNum += 1


if(stageNum == 3):
    with logStage(stageNum, "Unzip qt-everywhere-opensource-src-5.8.0.zip"):
        if(os.path.exists(QT_SRC_DIR)):
            warnings.warn(
                "~/qt-everywhere-opensource-src-5.8.0 already exists, deleting",
                RuntimeWarning
            )
            shutil.rmtree(QT_SRC_DIR)

        
        with zipfile.ZipFile(expanduser("~/qt-everywhere-opensource-src-5.8.0.zip")) as myzip:
            myzip.extractall(expanduser("~/"))
    stageNum += 1

if(stageNum == 4):
    with logStage(stageNum, "Create qmake.conf for cross-compilation"):
        srcDir = os.path.join(QT_SRC_DIR, "qtbase/mkspecs/linux-arm-gnueabi-g++")
        destDir = os.path.join(QT_SRC_DIR, "qtbase/mkspecs/linux-arm-gnueabihf-g++")
        if(os.path.exists(destDir)):
            warnings.warn(
                destDir + " already exists, deleting"
            )
            shutil.rmtree(destDir)
        print(" Copying linux-arm-gnueabi-g++ to linux-arm-gnueabihf-g++...")
        shutil.copytree(srcDir, destDir)
        print(" Modifying qmake.conf...")
        with fileinput.input(os.path.join(QT_SRC_DIR, "qtbase/mkspecs/linux-arm-gnueabihf-g++/qmake.conf"), inplace=True) as confFile:
            for line in confFile:
                line = line.replace("arm-linux-gnueabi", "arm-linux-gnueabihf")
                print(line, end="")
    stageNum += 1                

if(stageNum == 5):
    with logStage(stageNum, "Run configure"):
        buildDir = expanduser("~/qt-build")
        if(os.path.exists(buildDir)):
            warnings.warn(
                "~/qt-build already exists, deleting...",
                RuntimeWarning
            )
            shutil.rmtree(buildDir)
        os.makedirs(buildDir)
        os.chdir(buildDir)
        my_env = os.environ.copy()
        my_env["PATH"] = "C:\SysGCC\Raspberry\bin:" + my_env["PATH"]
        subprocess.call([os.path.join(QT_SRC_DIR,"configure.bat"), "/k","-platform","win32-g++","-xplatform","linux-arm-gnueabi-g++","-release","-opengl","es2","-device","linux-rpi3-vc4-g++","-sysroot","C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot","-prefix","/usr/local/qt5", "-opensource","-confirm-license"], env=my_env)
        
                             
                  
        
    

print("Script completed.")
    
    
