package com.example.mmapdemo

import android.app.Activity
import android.os.Bundle
import android.widget.Button
import com.example.mmap_lib.LogHelper

class MainActivity : Activity() {
    private var index = 0
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        LogHelper.init(this)
        findViewById<Button>(R.id.bt_save_log).setOnClickListener {
            LogHelper.logD(this, "text $index ,")
            index++
        }
    }


}