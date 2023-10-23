import os
import sys
import subprocess
from pathlib import Path

import Utils

class OpenCVConfiguration:
    requiredOpenCVVersion = ""  # Example: expecting a 4.5.x version
    installOpenCVVersion = "4.5.8"  # Example version
    openCVDirectory = "./vendor/OpenCV"

    @classmethod
    def Validate(cls):
        if (not cls.CheckOpenCV()):
            print("OpenCV not installed correctly.")
            return

        print("OpenCV configuration validated successfully!")

    @classmethod
    def CheckOpenCV(cls):
        opencv_dir = os.environ.get("OpenCV_LIB")
        if (opencv_dir is None):
            print("\nYou don't have OpenCV installed or OpenCV_LIB is not set!")
            cls.__InstallOpenCV()
            return False

        # Additional check to ensure correct version (you can expand on this)
        # For simplicity, it's just checking if the version string is present in the directory path
        if (cls.requiredOpenCVVersion not in opencv_dir):
            print(opencv_dir)
            print(f"You don't have the correct OpenCV version! (Engine requires {cls.requiredOpenCVVersion})")
            cls.__InstallOpenCV()
            return False
    
        print(f"Correct OpenCV version located at {opencv_dir}")
        return True

    @classmethod
    def __InstallOpenCV(cls):
        permissionGranted = False
        while not permissionGranted:
            reply = str(input(f"Would you like to install OpenCV {cls.installOpenCVVersion}? [Y/N]: ")).lower().strip()[:1]
            if reply == 'n':
                return
            permissionGranted = (reply == 'y')

        opencv_install_url = f"https://github.com/opencv/opencv/releases/download/{cls.installOpenCVVersion}/opencv-{cls.installOpenCVVersion}-vc14_vc15.exe"  # Adjust the URL pattern if needed
        opencv_path = f"{cls.openCVDirectory}/opencv-{cls.installOpenCVVersion}-installer.exe"
        
        print(f"Downloading {opencv_install_url} to {opencv_path}")
        Utils.DownloadFile(opencv_install_url, opencv_path)
        
        print("Running OpenCV installer...")
        os.startfile(os.path.abspath(opencv_path))
        print("Re-run this script after installation and ensure OpenCV_DIR environment variable is set correctly!")
        sys.exit()

if __name__ == "__main__":
    OpenCVConfiguration.Validate()
