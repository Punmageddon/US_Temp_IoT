#ifndef SerialHeartbeatTask_hpp
#define SerialHeartbeatTask_hpp

/**
 *  \brief Heartbeat task that regularly prints to Serial, indicating that the main loop is still running
 */
class SerialHeartbeatTask {
public:
  /**
   * \brief Call initially after Serial.begin(...) to set up task
   * 
   * \param interval The interval between heartbeat messages in milliseconds
   */
  void setup(unsigned long interval);
  
  /**
   * \brief Call once in main Arduino loop
   */
  void loop(void);
  
private:
  unsigned long interval;
  unsigned long millisNextHeartbeat;
};

#endif // SerialHeartbeatTask_hpp
