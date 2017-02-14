import urllib.request
import zipfile
import hashlib
import os
import shutil
import warnings
import fileinput
import subprocess
import ScriptStage
from sys import argv
from os.path import expanduser

# Constant Variable Definitions
QT_SRC_URL = "http://download.qt.io/official_releases/qt/5.8/5.8.0/single/qt-everywhere-opensource-src-5.8.0.zip"
QT_SRC_CHECKSUM = "c57cf81c1394230c5a188b7601bb4c072314cb350d5d3d6b5b820426c60570e5"
QT_SRC_DIR = expanduser("~/qt-everywhere-opensource-src-5.8.0") # Note: Should match directory of qt-everywhere-opensource-src-5.8.0.zip
QT_BUILD_DIR = expanduser("~/qt-build")

SCRIPT_DIR = os.path.dirname(__file__) # Path that this script is in.
STAGE_PATH = os.path.join(SCRIPT_DIR, "temp/CrossCompileQT5.stg")

# Detect start and end stage from command line arguments
start_stage = -1
end_stage = -1
if len(argv) > 1:
    start_stage = int(argv[1])
if len(argv) > 2:
    end_stage = int(argv[2])



# Adapted from https://visualgdb.com/tutorials/raspberry/qt/embedded/
ss = ScriptStage.ScriptStages(STAGE_PATH, start_stage, end_stage)

with ss.stage(1, "Download qt-everywhere-opensource-src-5.8.0.zip") as b:
    if b:
        with urllib.request.urlopen(QT_SRC_URL) as f, open(expanduser("~/qt-everywhere-opensource-src-5.8.0.zip"), "wb") as outfile:
            shutil.copyfileobj(f, outfile)

with ss.stage(2, "Check sha-256 hash of downloaded file.") as b:
    if b:
        with open(expanduser("~/qt-everywhere-opensource-src-5.8.0.zip"), "rb") as myFile:
            hash_sha256 = hashlib.sha256()
            for chunk in iter(lambda: myFile.read(4096), b""):
                hash_sha256.update(chunk)
            if hash_sha256.hexdigest() != QT_SRC_CHECKSUM:
                raise RuntimeError("SHA-256 hash for qt-everywhere-opensource-src-5.8.0.zip does not match expected value.")

with ss.stage(3, "Unzip qt-everywhere-opensource-src-5.8.0.zip") as b:
    if b:
        if os.path.exists(QT_SRC_DIR):
            warnings.warn("~/qt-everywhere-opensource-src-5.8.0 already exists, deleting...")
            shutil.rmtree(QT_SRC_DIR)
        print("Expanding zipfile...")
        with zipfile.ZipFile(expanduser("~/qt-everywhere-opensource-src-5.8.0.zip")) as myzip:
            myzip.extractall(expanduser("~/"))


with ss.stage(4, "Create qmake.conf for cross-compilation") as b:
    if b:
        srcDir = os.path.join(QT_SRC_DIR, "qtbase/mkspecs/linux-arm-gnueabi-g++")
        destDir = os.path.join(QT_SRC_DIR, "qtbase/mkspecs/linux-arm-gnueabihf-g++")
        if os.path.exists(destDir):
            warnings.warn(destDir + " already exists, deleting...")
            shutil.rmtree(destDir)
        print(" Copying linux-arm-gnueabi-g++ to linux-arm-gnueabihf-g++...")
        shutil.copytree(srcDir, destDir)
        print(" Modifying linux-arm-gnueabihf-g++/qmake.conf...")
        with fileinput.input(os.path.join(QT_SRC_DIR, "qtbase/mkspecs/linux-arm-gnueabihf-g++/qmake.conf"), inplace=True) as confFile:
            for line in confFile:
                line = line.replace("arm-linux-gnueabi", "arm-linux-gnueabihf")
                print(line, end="")

with ss.stage(5, "Patching wint32-g++\qmake.conf") as b:
    if b:
        source_path = os.path.join(QT_SRC_DIR, "qtbase/mkspecs/win32-g++/qmake.conf")
        backup_path = os.path.join(QT_SRC_DIR, "qtbase/mkspecs/win32-g++/qmake.conf.bak")

        if not os.path.exists(backup_path):
            print("Backup configure not found, creating it...")
            shutil.copy(source_path, backup_path)

        print("Patching ", source_path, " ...")

        with fileinput.input(backup_path) as configure_file, open(source_path, "w") as source_file:
            for line in configure_file:
                if 'QMAKE_CXXFLAGS          = $$QMAKE_CFLAGS' in line:
                    source_file.write("QMAKE_CXXFLAGS          = $$QMAKE_CFLAGS -U__STRICT_ANSI__\n")
                else:
                    source_file.write(line)

with ss.stage(6, "Run configure") as b:
    if b:

        if os.path.exists(QT_BUILD_DIR):
            warnings.warn("~/qt-build already exists, deleting...")
            shutil.rmtree(QT_BUILD_DIR)

        os.makedirs(QT_BUILD_DIR)
        os.chdir(QT_BUILD_DIR)
        my_env = os.environ.copy()
        my_env["PATH"] = "C:/SysGCC/Raspberry/bin:" + my_env["PATH"]
        subprocess.call([os.path.join(QT_SRC_DIR,"configure.bat"),"-platform","win32-g++","-xplatform","linux-arm-gnueabihf-g++","-release","-opengl","es2","-device","linux-rpi3-g++","-sysroot","C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot","-prefix","/usr/local/qt5", "-opensource","-confirm-license"], env=my_env)

with ss.stage(7, "Check qmake.exe was built") as b:
    if b:
        qmake_path = os.path.join(QT_BUILD_DIR, "qtbase/bin/qmake.exe")
        if not os.path.exists(qmake_path):
            raise RuntimeError("qmake.exe not found, script cannot continue.")
        subprocess.call([qmake_path, "-v"])

with ss.stage(8, "Backing up and patching qtbase/configure") as b:
    if b:
        source_path = os.path.join(QT_SRC_DIR, "qtbase/configure")
        backup_path = os.path.join(QT_SRC_DIR, "qtbase/configure.bak")

        if not os.path.exists(backup_path):
            print("Backup configure not found, creating it...")
            shutil.copy(source_path, backup_path)

        print("Patching",source_path, " ...")

        with fileinput.input(backup_path) as configure_file, open(source_path, "w") as source_file:
            for line in configure_file:
                if 'echo "Creating qmake..."' in line:
                    source_file.write("'!' -f \"$outpath/bin/qmake.exe\"\n")
                source_file.write(line)

with ss.stage(9, "Rerun configure with cross-compilation") as b:
    if b:
        qmake_path = expanduser("~/qt-build/qtbase/bin/qmake.exe")
        if not os.path.exists(qmake_path):
            raise RuntimeError("qmake.exe not found, script cannot continue.")

        os.chdir(QT_BUILD_DIR)

        my_env = os.environ.copy()
        my_env["PATH"] = "C:/SysGCC/Raspberry/bin:" + my_env["PATH"]
        subprocess.call(
            [os.path.join(QT_SRC_DIR, "configure.bat"), "-platform", "win32-g++", "-xplatform", "linux-arm-gnueabihf-g++",
             "-release", "-opengl", "es2", "-device", "linux-rpi3-g++", "-sysroot",
             "C:/SysGCC/Raspberry/arm-linux-gnueabihf/sysroot", "-prefix", "/usr/local/qt5", "-opensource",
             "-confirm-license", "-device-option",
             "CROSS_COMPILE=C:/SysGCC/Raspberry/bin/arm-linux-gnueabihf-", "-qt-xcb"], env=my_env)

with ss.stage(10, "Run make && make-install") as b:
    if b:
        os.chdir(QT_BUILD_DIR)
        subprocess.call(["make.exe", "-j4"])

print("Script completed.")
