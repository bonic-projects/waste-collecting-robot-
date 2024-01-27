class DeviceReading {
  int f1;
  int f2;
  int f3;
  int f4;
  double leftS;
  double rightS;
  double frontS;
  int metalSensorValue;
  DateTime lastSeen;

  DeviceReading({
    required this.f1,
    required this.f2,
    required this.f3,
    required this.f4,
    required this.leftS,
    required this.rightS,
    required this.frontS,
    required this.metalSensorValue,
    required this.lastSeen,
  });

  factory DeviceReading.fromMap(Map data) {
    return DeviceReading(
      f1: data['f1']  ?? 0,
      f2: data['f2']  ?? 0,
      f3: data['f3']  ?? 0,
      f4: data['f4']  ?? 0,
      leftS: data['leftS'] != null ? data['leftS'].toDouble() : 0.0,
      rightS: data['rightS'] != null ? data['rightS'].toDouble() : 0.0,
      frontS: data['frontS'] != null ? data['frontS'].toDouble() : 0.0,
      metalSensorValue: data['metalSensorValue'] ?? 0,
      lastSeen: DateTime.fromMillisecondsSinceEpoch(data['ts']),
    );
  }
}



class DeviceData {
  bool isForward;
  bool isBackward;
  bool isLeft;
  bool isRight;
  bool isLeftSide;
  bool isRightSide;
  bool isStop;
  bool isAutomatic;
  int servo1;
  int servo2;
  int servo3;
  int servo4;
  int servo5;
  int servo6;

  DeviceData({
    required this.isForward,
    required this.isBackward,
    required this.isLeft,
    required this.isRight,
    required this.isLeftSide,
    required this.isRightSide,
    required this.isStop,
    required this.isAutomatic,
    required this.servo1,
    required this.servo2,
    required this.servo3,
    required this.servo4,
    required this.servo5,
    required this.servo6,
  });

  factory DeviceData.fromMap(Map data) {
    return DeviceData(
      isForward: data['isForward'] ?? false,
      isBackward: data['isBackward'] ?? false,
      isLeft: data['isLeft'] ?? false,
      isRight: data['isRight'] ?? false,
      isLeftSide: data['isLeftSide'] ?? false,
      isRightSide: data['isRightSide'] ?? false,
      isStop: data['isStop'] ?? false,
      isAutomatic: data['isAutomatic'] ?? false,
      servo1: data['servo1'] ?? 0,
      servo2: data['servo2'] ?? 0,
      servo3: data['servo3'] ?? 0,
      servo4: data['servo4'] ?? 0,
      servo5: data['servo5'] ?? 0,
      servo6: data['servo6'] ?? 0,
    );
  }


  Map<String, dynamic> toJson() {
    return {
      'isForward': isForward,
      'isBackward': isBackward,
      'isLeft': isLeft,
      'isRight': isRight,
      'isLeftSide': isLeftSide,
      'isRightSide': isRightSide,
      'isAutomatic': isAutomatic,
      'isStop': isStop,
      'servo1': servo1,
      'servo2': servo2,
      'servo3': servo3,
      'servo4': servo4,
      'servo5': servo5,
      'servo6': servo6,
    };
  }
}
