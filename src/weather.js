var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // We will request the weather here
  var root = 'http://api.wunderground.com/api/c460e181460b9060';
  var end = 'ws:0/q/' + pos.coords.latitude + ',' + pos.coords.longitude + '.json';
  var temp = root + '/conditions/' + end;
  var forecast = root + '/forecast/' + end;
  
  var dictionary = {
        'KEY_TEMPERATURE': '',
        'KEY_HIGH': ''
      };
  
  // Send request to Wunderground
  xhrRequest(temp, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Get temp
      var temperature = json.current_observation.temp_f;
      console.log('Temperature is F' + temperature);
      
      // Add to dictionary
      dictionary.KEY_TEMPERATURE = +temperature;
      
      xhrRequest(forecast, 'GET', 
        function(responseText) {
          // responseText contains a JSON object with weather info
          var json = JSON.parse(responseText);
    
          // Get temp
          var temperature = json.forecast.simpleforecast.forecastday[0].high.fahrenheit;
          console.log('High is F' + temperature);
          
          // Add to dictionary
          dictionary.KEY_HIGH = +temperature;
          
          Send(dictionary);
        }      
      ); 
    }      
  ); 
}

function Send(dictionary) {
  // Send to Pebble
  console.log(JSON.stringify(dictionary));
  Pebble.sendAppMessage(dictionary,
                        function(e) {
                          console.log('Weather info sent to Pebble successfully!');
                        },
                        function(e) {
                          console.log('Error sending weather info to Pebble!');
                        }
                       );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');

    // Get the updated weather
    getWeather();
  }                     
);