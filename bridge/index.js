const WebSocket = require("ws");
const rclnodejs = require("rclnodejs");
const EventEmitter = require("node:events");

const wss = new WebSocket.Server({ port: 2081 });
const eventEmitter = new EventEmitter();

// Create a ROS node and then print out the string message received from publishers
rclnodejs.init().then(() => {
  const node = new rclnodejs.Node("shut_up");
  node.createSubscription(
    "optimate_interfaces/msg/OptibusGps",
    "optibus_gps",
    (msg) => {
      console.log(`Received message: ${typeof msg}`, msg);
      eventEmitter.emit("ros_data", Date(), msg);
    }
  );

  node.spin();
});

// Websocket Server
wss.on("connection", (ws, req) => {
  // Callback to manage ros_data payload
  function rosCb(date, msg) {
    console.log("ros data!", date, msg);

    ws.send(
      JSON.stringify({
        topic: "ros_data",
        content: msg,
      })
    );
  }

  // Add ros_data event listerner
  eventEmitter.on("ros_data", rosCb);

  // Remove ros_data event listener when the ws connection is closed
  ws.on("close", (current_ws, req) => {
    eventEmitter.off("ros_data", rosCb);
  });
});
