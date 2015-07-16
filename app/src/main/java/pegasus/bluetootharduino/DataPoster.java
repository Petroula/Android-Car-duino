package pegasus.bluetootharduino;

import android.util.Log;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by dimi on 16/7/2015.
 */
public class DataPoster implements Runnable {

    public DataPoster(){
        Thread t = new Thread(this);
        t.start();
    }

    public DataPoster(String sensorType, String sensorNumber, int value) {
    }

    @Override
    public void run() {
        httpPostRequest();
    }

    private String httpPostRequest() {

        InputStream inputStream = null;
        String result = "";
        try {

            // 1. create HttpClient
            HttpClient httpclient = new DefaultHttpClient();

            // 2. make POST request to the given URL
            HttpPost httpPost = new HttpPost("http://ec2-54-172-35-25.compute-1.amazonaws.com/publish");

            String json = "";

            // 3. build jsonObject
            JSONObject totalPost = new JSONObject();
            totalPost.put("topic", "interchange.vehicle.1.stream");

            JSONArray args = new JSONArray();
            JSONObject telematics = new JSONObject();
            telematics.put("fuel_level", "10");
            telematics.put("battery_level", "10");
            telematics.put("speed", "30");
            telematics.put("heading", "NORTH");

            JSONObject lights = new JSONObject();
            lights.put("head",true);      // head,tail,...
            lights.put("tail", false);
            lights.put("brake",false);

            JSONObject signal = new JSONObject();
            signal.put("right",false); // right, left,...
            signal.put("left", true);

            lights.put("signal",signal);
            telematics.put("lights",lights);

            JSONObject infraredSensors = new JSONObject();
            infraredSensors.put("sensor1","14");
            infraredSensors.put("sensor2", "22");
            infraredSensors.put("sensor3", "4");

            JSONObject ultrasonicSensors = new JSONObject();
            ultrasonicSensors.put("sensor1","14");
            ultrasonicSensors.put("sensor2", "22");
            ultrasonicSensors.put("sensor3", "4");

            JSONObject periferalSensors = new JSONObject();
            periferalSensors.put("infrared",infraredSensors);
            periferalSensors.put("ultrasonic",infraredSensors);

            telematics.put("periferal_sensors", periferalSensors);
            JSONObject telematicsArray = new JSONObject();
            telematicsArray.put("telematics",telematics);
            args.put(telematicsArray);
            totalPost.put("args", args);

            json = totalPost.toString();

            // ** Alternative way to convert Person object to JSON string usin Jackson Lib
            // ObjectMapper mapper = new ObjectMapper();
            // json = mapper.writeValueAsString(person);

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
