
#include <cstdio>


#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "optimate_interfaces/msg/optibus_state.hpp"
#include "optimate_interfaces/msg/optibus_control_manual.hpp"
#include "optimate_interfaces/msg/optibus_control_state.hpp"
#include "optimate_interfaces/msg/optibus_wpts_control.hpp"
#include "optimate_interfaces/msg/optibus_wpts_data.hpp"
#include "optimate_interfaces/msg/optibus_wpts_status.hpp"

#include "optimate_interfaces/msg/optibus_gps.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;

class Chatter : public rclcpp::Node
{
  public :

    Chatter()
      : Node("chatter"), count_(0)
      {
        state_publisher_ = this->create_publisher<optimate_interfaces::msg::OptibusGps>("optibus_gps", 10);
        timer_ = this->create_wall_timer(
        10ms, std::bind(&Chatter::timer_callback, this));

        manual_subscription_ = this->create_subscription<optimate_interfaces::msg::OptibusControlManual>(
        "optibus_control_manual", 10, std::bind(&Chatter::optibus_state_callback, this, _1));
      }
  private :

    void timer_callback()
    {
      auto message=optimate_interfaces::msg::OptibusGps();
      message.latitude=10;
      message.longitude=10.;
      RCLCPP_INFO(this->get_logger(), "Message sent with timer");
      state_publisher_->publish(message);
    }
    void optibus_state_callback(const optimate_interfaces::msg::OptibusControlManual & msg)
    {
      auto message=optimate_interfaces::msg::OptibusState();
      message.current_state=55;
      message.brake_demand_ce=55;
      message.throttle_demand_ce=55;
      message.message_counter_ce=55;
      message.status_word=55;
      RCLCPP_INFO(this->get_logger(), "Message sent as response");
      //state_publisher_->publish(message);
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<optimate_interfaces::msg::OptibusGps>::SharedPtr state_publisher_;
    rclcpp::Subscription<optimate_interfaces::msg::OptibusControlManual>::SharedPtr manual_subscription_;
    size_t count_;
};



int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Chatter>());
  rclcpp::shutdown();
  return 0;
}