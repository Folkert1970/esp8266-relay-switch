void setup_switch(){
  for (int i = 0; i < sizeof(switch_pins); i++){
      pinMode(switch_pins[i], OUTPUT);
  }
  // switch_states[] = {0, 0}; //reset switch states upon starting
  switch_relay();
}

void switch_relay (int id){
  switch_states[id] = 1 - switch_states[id];
  digitalWrite(switch_pins[id], switch_states[id]);
}

void switch_relay (int id, int state){
  switch_states[id] = state;
  digitalWrite(switch_pins[id], switch_states[id]);
}

void switch_relay (){
  for (int i = 0; i < sizeof(switch_pins); i++){
    digitalWrite(switch_pins[i], switch_states[i]);
  }
}

int switch_status(int id){
  return switch_states[id];
}
