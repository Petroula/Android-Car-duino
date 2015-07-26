package pegasus.bluetootharduino;

import android.util.Log;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by dimi on 16/7/2015.
 * HTTP POST request code based on: http://hmkcode.com/android-send-json-data-to-server/
 * Posts data from the car, to the Ericsson connected vehicle cloud.
 * API: http://ec2-54-172-35-25.compute-1.amazonaws.com/docs/
 */
public class DataPoster implements Runnable {
    private JSONObject telematics;

    //peripheral sensors (and lights) constructor (sensors, lights)
    public DataPoster(String sensorType, String sensorID, int value) {
        postJSON(buildJSON(sensorType, sensorID, value));
    }

    //vehicle data constructor (speed and heading)
    public DataPoster(String sensorType, String value) {
        postJSON(buildJSON(sensorType, value));
    }

    private JSONObject buildJSON(String sensorType, String sensorID, int value){

        JSONObject sensorContents = new JSONObject();
        Object val = value; //Object since we do not know if it
        if (sensorID.equalsIgnoreCase("lights")){
            val = (value == 1); //convert 0,1 to false,true
        }
        try {
            JSONObject sensor = new JSONObject();
            sensor.put(sensorID,val);
            sensorContents.put(sensorType,sensor);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return sensorContents;
    }

    private JSONObject buildJSON(String sensorType, String value){

        JSONObject sensorContents = new JSONObject();
        Object val = value;
        if (sensorType.equalsIgnoreCase("speed")) { //if it is speed, it should be an integer
            val = Integer.parseInt(value);
        }
        try {
            sensorContents.put(sensorType,val);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return sensorContents;
    }

    private void postJSON(JSONObject vehicleData) {
        telematics = new JSONObject();
        try {
            telematics.put("telematics",vehicleData);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        beginThread();
    }
    private void beginThread(){
        Thread t = new Thread(this);
        t.start();
    }

    @Override
    public void run() {
        httpPostRequest();
    }

    private String httpPostRequest() {

        InputStream inputStream = null;
        String result = "";
        final String serverURL = "http://ec2-54-172-35-25.compute-1.amazonaws.com/publish";
        try {

            // 1. create HttpClient
            HttpClient httpclient = new DefaultHttpClient();

            // 2. make POST request to the given URL
            HttpPost httpPost = new HttpPost(serverURL);

            String json = "";

            // 3. build jsonObject
            JSONObject totalPost = new JSONObject();
            totalPost.put("topic", "interchange.vehicle.1.stream");

            JSONArray args = new JSONArray();

            args.put(telematics);
            totalPost.put("args", args);

            json = totalPost.toString();

            // 5. set json to StringEntity
            StringEntity se = new StringEntity(json);

            // 6. set httpPost Entity
            httpPost.setEntity(se);

            // 7. Set some headers to inform server about the type of the content
            httpPost.setHeader("Accept", "application/json");
            httpPost.setHeader("Content-type", "application/json");

            // 8. Execute POST request to the given URL
            HttpResponse httpResponse = httpclient.execute(httpPost);

            // 9. receive response as inputStream
            inputStream = httpResponse.getEntity().getContent();

            // 10. convert inputstream to string
            if(inputStream != null)
                result = convertInputStreamToString(inputStream);
            else
                result = "Did not work!";

        } catch (Exception e) {
            Log.d("InputStream", e.getLocalizedMessage());
        }
        Log.d("POST result:",result);
        // 11. return result
        return result;
    }

    private String convertInputStreamToString(InputStream inputStream) throws IOException {
        BufferedReader bufferedReader = new BufferedReader( new InputStreamReader(inputStream));
        String line = "";
        String result = "";
        while((line = bufferedReader.readLine()) != null)
            result += line;

        inputStream.close();
        return result;

    }


}
