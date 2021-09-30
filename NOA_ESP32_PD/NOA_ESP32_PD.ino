#include <Wire.h>
#include "tcpm_driver.h"
#include "usb_pd.h"

#define ESP32DEBUGGING
#include <ESP32Logger.h>

const int usb_pd_int_pin = 32;
const int debug_led_pin  = 33;
const int pushbutton_pin = 35;
int pushbutton_last_state, pushbutton_current_state, pushbutton_last_time, pushbutton_current_time;
int debug_led_current_state = 0;
int pd_source_cap_current_index = 0, pd_source_cap_max_index = 0;

// USB-C Specific - TCPM start 1
const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT] = {
  {0, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv},
};
// USB-C Specific - TCPM end 1

void setup() {
  Serial.begin(115200);
  delay(50);
  // Initialize debugging with timestamp since start (start = 0) 
  DBGINI(&Serial, ESP32Timestamp::TimestampSinceStart)
  // Start debugging
  DBGSTA
  // Log an info-message
  DBGLOG(Info, "Program ExampleESP32Debugging.ino startet.")
  // Log an error-message
  DBGLOG(Error, "This is an error message.")
  // Log a debug-message, which will not be shown, because actual debug-level is "Info"
  DBGLOG(Debug, "This is an message with logging level 'Debug' which will not be printed.")
  // Change debug-level to "Debug"
  DBGLEV(Debug)
  // The following debug-message will now be shown
  DBGLOG(Debug, "This is an message with logging level 'Debug' which will be printed.")
  // Stop debugging
  DBGSTP
  // While debugging is stopped messages will not be shown
  DBGLOG(Info, "This will not be printed because Debugging is stopped.")
  // Activate debugging again
  DBGSTA

  pinMode(usb_pd_int_pin, INPUT);
  pinMode(debug_led_pin, OUTPUT);
  pinMode(pushbutton_pin, INPUT_PULLUP);
  pushbutton_last_state = 1;
  pushbutton_last_time = 0;
  digitalWrite(debug_led_pin, LOW);
  
  Wire.begin();
//  Wire.begin(26,27);
  Wire.setClock(100000);
  
  tcpm_init(0);
  delay(50);
  pd_init(0);
  delay(50);
}

void loop() {
  int reset = 0;
  pushbutton_current_state = digitalRead(pushbutton_pin);
  pushbutton_current_time = millis();
  if ((pushbutton_current_state == 0) && (pushbutton_last_state == 1) && ((pushbutton_current_time - pushbutton_last_time) < 100)) {
    if (debug_led_current_state) {
      digitalWrite(debug_led_pin, LOW);
      debug_led_current_state = 0;
    } else {
      digitalWrite(debug_led_pin, HIGH);
      debug_led_current_state = 1;
    }
    if (pd_source_cap_current_index < pd_source_cap_max_index) {
      pd_source_cap_current_index++;
    } else {
      pd_source_cap_current_index = 0;
    }
    //pd[0].task_state = PD_STATE_SOFT_RESET);
    reset = 1;
  }
  pushbutton_last_state = pushbutton_current_state;
  pushbutton_last_time = pushbutton_current_time;
  
  if (LOW == digitalRead(usb_pd_int_pin)) {
      tcpc_alert(0);
  }
  
  pd_run_state_machine(0, reset);
  // For some reason, a delay of 4 ms seems to be best
  // My guess is that spamming the I2C bus too fast causes problems
  delay(4);
}

void pd_process_source_cap_callback(int port, int cnt, uint32_t *src_caps)
{
  //digitalWrite(debug_led_pin, HIGH);
  pd_source_cap_max_index = cnt - 1;
}
