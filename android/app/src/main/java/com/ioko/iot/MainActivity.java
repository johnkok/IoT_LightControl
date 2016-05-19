package com.ioko.iot;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.StrictMode;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class MainActivity extends Activity {
Context m;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        m = this;
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        Button A_OFF = (Button)findViewById(R.id.buttonAoff);
        Button A_ON = (Button)findViewById(R.id.buttonAon);
        Button B_OFF = (Button)findViewById(R.id.buttonBoff);
        Button B_ON = (Button)findViewById(R.id.buttonBon);
        Button C_OFF = (Button)findViewById(R.id.buttonCoff);
        Button C_ON = (Button)findViewById(R.id.buttonCon);
        Button EXIT = (Button)findViewById(R.id.buttonExit);

        A_OFF.setOnClickListener(buttonHandler);
        A_ON.setOnClickListener(buttonHandler);
        B_OFF.setOnClickListener(buttonHandler);
        B_ON.setOnClickListener(buttonHandler);
        C_OFF.setOnClickListener(buttonHandler);
        C_ON.setOnClickListener(buttonHandler);
        EXIT.setOnClickListener(buttonHandler);

    }

    View.OnClickListener buttonHandler = new View.OnClickListener() {

        public void onClick(View v) {
            byte data = 0;
            switch(v.getId()){
                case (R.id.buttonAoff):
                    data = 0x10;  // Light A off
                    break;
                case (R.id.buttonAon):
                    data = 0x11;  // Light A on
                    break;
                case (R.id.buttonBoff):
                    data = 0x20;  // Light B off
                    break;
                case (R.id.buttonBon):
                    data = 0x21;  // Light B on
                    break;
                case (R.id.buttonCoff):
                    data = 0x30;  // Light C off
                    break;
                case (R.id.buttonCon):
                    data = 0x31;  // Light C on
                    break;

                case (R.id.buttonExit):
                    finish();
                    break;
            }

            byte[] a = new byte[5];

            a[0] = 0x19; //Preamp word A
            a[1] = 0x78; //Preamp word B
            a[2] = data;
            a[3] = 0x00;
            a[4] = (byte)(a[0] + a[1] + a[2] + a[3]);  //Checksum

            Socket s;
            OutputStream o;
            InputStream i;
            try {
                s = new Socket("MyHost.com", 8888);
                o = s.getOutputStream();
                o.write(a,0,5);
                i = s.getInputStream();
                i.read(a,0,1);

            } catch (IOException e) {
                e.printStackTrace();
            }

        }
    };
}
