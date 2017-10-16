package urbetter.usbcam;

/**
 * Created by xiao on 2017/9/22.
 */

public class UsbCamControl {
    public static native int CamOpen(String name,int w,int h);
    public static native void CamClose(int fd);
    public static native int CamGetWidth(int fd);
    public static native int CamGetHeight(int fd);
    public static native byte[] CamGetData(int fd);
    static {
        System.loadLibrary("UsbCam_jni");
    }
}
