package com.example.mmap_lib;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import java.io.File;

public class LogHelper {

    private static final String FOLDER_NAME = "log_folder";
    private static final String LOG_FILE_NAME = "log.txt";

    private static LogHelper sLogHelper = new LogHelper();
    private static String defaultBufferPath = null;
    private static Context mContext = null;


    static {
        System.loadLibrary("mmap-lib");
    }

    private LogHelper() {

    }

    public static void init(Context context) {
        mContext = context;
        defaultBufferPath = getDefaultBufferPath(mContext);
        initNative(defaultBufferPath, 4096, "8888888888");
    }

    private static String getDefaultBufferPath(Context context) {
        File bufferFile;
        if (Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED)
                && context.getExternalFilesDir(FOLDER_NAME) != null) {
            bufferFile = context.getExternalFilesDir(FOLDER_NAME);
        } else {
            bufferFile = new File(context.getFilesDir(), FOLDER_NAME);
        }
        if (bufferFile != null && !bufferFile.exists()) {
            bufferFile.mkdirs();
        }
        return new File(bufferFile, LOG_FILE_NAME).getAbsolutePath();
    }

    public static void logD(Context context, String msg) {
        sLogHelper.writeNative(0, msg);
    }


    private native static long initNative(String bufferPath, int capacity, String logPath);

    private native void writeNative(long ptr, String log);

}
