import 'package:stacked/stacked.dart';
import 'package:stacked_services/stacked_services.dart';

import '../../../app/app.locator.dart';
import '../../../app/app.logger.dart';
import '../../../models/device.dart';
import '../../../services/database_service.dart';

class HomeViewModel extends ReactiveViewModel {
  final log = getLogger('HomeViewModel');

  // final _snackBarService = locator<SnackbarService>();
  final _navigationService = locator<NavigationService>();
  final _dbService = locator<DatabaseService>();

  DeviceReading? get data => _dbService.node;

  @override
  List<ListenableServiceMixin> get listenableServices => [_dbService];

  void onModelReady() {
    getDeviceData();
  }

  void openAutomaticView() {
    // _navigationService.navigateToAutomaticView();
  }

  DeviceData _deviceData = DeviceData(isForward: false, isBackward: false, isLeft: false, isRight: false, isLeftSide: false, isRightSide: false, isStop: false, servo1: 0, servo2: 0, servo3: 0, servo4: 0, servo5: 0, servo6: 0, isAutomatic: false);
  DeviceData get deviceData => _deviceData;

  void setStop() {
    _deviceData.isStop = true;
    _deviceData.isForward = false;
    _deviceData.isBackward = false;
    _deviceData.isLeft = false;
    _deviceData.isRight = false;
    _deviceData.isLeftSide = false;
    _deviceData.isRightSide = false;
    _deviceData.isAutomatic = false;
    setDeviceData();
    notifyListeners();
  }

  void setForward() {
    _deviceData.isStop = false;
    _deviceData.isForward = true;
    _deviceData.isBackward = false;
    _deviceData.isLeft = false;
    _deviceData.isRight = false;
    _deviceData.isLeftSide = false;
    _deviceData.isRightSide = false;
    _deviceData.isAutomatic = false;
    setDeviceData();
    notifyListeners();
  }

  void setLeft() {
    _deviceData.isStop = false;
    _deviceData.isForward = false;
    _deviceData.isBackward = false;
    _deviceData.isLeft = true;
    _deviceData.isRight = false;
    _deviceData.isLeftSide = false;
    _deviceData.isRightSide = false;
    _deviceData.isAutomatic = false;
    setDeviceData();
    notifyListeners();
  }

  void setRight() {
    _deviceData.isStop = false;
    _deviceData.isForward = false;
    _deviceData.isBackward = false;
    _deviceData.isLeft = false;
    _deviceData.isRight = true;
    _deviceData.isLeftSide = false;
    _deviceData.isRightSide = false;
    _deviceData.isAutomatic = false;
    setDeviceData();
    notifyListeners();
  }

  void setBack() {
    _deviceData.isStop = false;
    _deviceData.isForward = false;
    _deviceData.isBackward = true;
    _deviceData.isLeft = false;
    _deviceData.isRight = false;
    _deviceData.isLeftSide = false;
    _deviceData.isRightSide = false;
    _deviceData.isAutomatic = false;
    setDeviceData();
    notifyListeners();
  }

  void setRightSide() {
    _deviceData.isStop = false;
    _deviceData.isForward = false;
    _deviceData.isBackward = false;
    _deviceData.isLeft = false;
    _deviceData.isRight = false;
    _deviceData.isLeftSide = false;
    _deviceData.isRightSide = true;
    _deviceData.isAutomatic = false;
    setDeviceData();
    notifyListeners();
  }

  void setLeftSide() {
    _deviceData.isStop = false;
    _deviceData.isForward = false;
    _deviceData.isBackward = false;
    _deviceData.isLeft = false;
    _deviceData.isRight = false;
    _deviceData.isLeftSide = true;
    _deviceData.isRightSide = false;
    _deviceData.isAutomatic = false;
    setDeviceData();
    notifyListeners();
  }

  void setAutomatic() {
    _deviceData.isStop = false;
    _deviceData.isForward = false;
    _deviceData.isBackward = false;
    _deviceData.isLeft = false;
    _deviceData.isRight = false;
    _deviceData.isLeftSide = false;
    _deviceData.isRightSide = false;
    _deviceData.isAutomatic = !_deviceData.isAutomatic;
    setDeviceData();
    notifyListeners();
  }



  void setServo1(int value){
    _deviceData.servo1 = value;
    setDeviceData();
  }
  void setServo2(int value){
    _deviceData.servo2 = value;
    setDeviceData();
  }  void setServo3(int value){
    _deviceData.servo3 = value;
    setDeviceData();
  }  void setServo4(int value){
    _deviceData.servo4 = value;
    setDeviceData();
  }  void setServo5(int value){
    _deviceData.servo5 = value;
    setDeviceData();
  } void setServo6(int value){
    _deviceData.servo6 = value;
    setDeviceData();
  }

  void setDeviceData() {
    _dbService.setDeviceData(_deviceData);
  }


  void getDeviceData() async {
    setBusy(true);
    DeviceData? deviceData = await _dbService.getDeviceData();
    if (deviceData != null) {
      _deviceData = DeviceData(isForward: deviceData.isForward,
        isBackward: deviceData.isBackward,
        isLeft: deviceData.isLeft,
        isRight: deviceData.isRight,
        isLeftSide: deviceData.isLeftSide,
        isRightSide: deviceData.isRightSide,
        isStop: deviceData.isStop,
        servo1: deviceData.servo1,
        servo2: deviceData.servo2,
        servo3: deviceData.servo3,
        servo4: deviceData.servo4,
        servo5: deviceData.servo5,
        servo6: deviceData.servo6,
        isAutomatic: deviceData.isAutomatic,
      );
      setBusy(false);
    }
  }
}