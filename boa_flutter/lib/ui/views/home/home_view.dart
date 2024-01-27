import 'dart:ui';

import 'package:flutter/material.dart';
import 'package:lottie/lottie.dart';
import 'package:stacked/stacked.dart';
import 'package:boa_flutter/ui/common/app_colors.dart';
import 'package:boa_flutter/ui/common/ui_helpers.dart';

import '../../smart_widgets/online_status.dart';
import 'home_viewmodel.dart';

class HomeView extends StackedView<HomeViewModel> {
  const HomeView({Key? key}) : super(key: key);

  @override
  Widget builder(
    BuildContext context,
    HomeViewModel viewModel,
    Widget? child,
  ) {
    return Scaffold(
        appBar: AppBar(
          title: const Text('BOA'),
          centerTitle: true,
          actions: const [IsOnlineWidget()],
        ),
        body:  _HomeBody(),
    );
  }

  @override
  HomeViewModel viewModelBuilder(
    BuildContext context,
  ) =>
      HomeViewModel();

  @override
  void onViewModelReady(HomeViewModel viewModel) {
    viewModel.onModelReady();
    super.onViewModelReady(viewModel);
  }
}

class _HomeBody extends ViewModelWidget<HomeViewModel> {
  const _HomeBody({Key? key}) : super(key: key, reactive: true);

  @override
  Widget build(BuildContext context, HomeViewModel model) {
    return Stack(
      children: [
        SizedBox(
          height: MediaQuery.of(context).size.height,
          child: SingleChildScrollView(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.stretch,
              children: [
                // CsButton(
                //     onTap: model.openAutomaticView,
                //     text: "Open automatic movement"),
                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("F1: ${model.data?.f1}"),
                  ),
                ),
                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("F2: ${model.data?.f2}"),
                  ),
                ),
                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("F3: ${model.data?.f3}"),
                  ),
                ),
                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("F4: ${model.data?.f4}"),
                  ),
                ),

                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("FrontS: ${model.data?.frontS}"),
                  ),
                ),
                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("RightS: ${model.data?.rightS}"),
                  ),
                ),
                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("LeftS: ${model.data?.leftS}"),
                  ),
                ),
                Card(
                  child: Padding(
                    padding: const EdgeInsets.all(18.0),
                    child: Text("Metal Sensor: ${model.data?.metalSensorValue}"),
                  ),
                ),

                Padding(
                  padding: const EdgeInsets.all(8.0),
                  child: Column(
                    // mainAxisAlignment: MainAxisAlignment.,
                    crossAxisAlignment: CrossAxisAlignment.stretch,
                    children: [
                      Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                        ConditionButton(onTap: model.setLeftSide, text: 'Left S', isTrue: model.deviceData.isLeftSide),
                        // ConditionButton(onTap: model.setStop, text: 'Stop', isTrue: model.deviceData.isStop),
                        ConditionButton(onTap: model.setRightSide, text: 'Right S', isTrue: model.deviceData.isRightSide),

                      ],),
                      const SizedBox(height: 20),
                      ConditionButton(onTap: model.setForward, text: 'Forward', isTrue: model.deviceData.isForward),
                      const SizedBox(height: 20),
                      Padding(
                        padding: const EdgeInsets.all(8.0),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceBetween,
                          children: [
                            ConditionButton(onTap: model.setLeft, text: 'Left', isTrue: model.deviceData.isLeft),
                            Padding(
                              padding: const EdgeInsets.all(8.0),
                              child: Row(
                                children: [
                                  const SizedBox(width: 10),
                                  ConditionButton(onTap: model.setStop, text: 'Stop', isTrue: model.deviceData.isStop),
                                ],
                              ),
                            ),
                            ConditionButton(onTap: model.setRight, text: 'Right', isTrue: model.deviceData.isRight),
                          ],
                        ),
                      ),
                      const SizedBox(height: 20),
                      ConditionButton(onTap: model.setBack, text: 'Backward', isTrue: model.deviceData.isBackward),
                    ],
                  ),
                ),
                ConditionButton(onTap: model.setAutomatic, text: 'Automatic', isTrue: model.deviceData.isAutomatic),

                if(!model.isBusy)
                Padding(
                  padding: const EdgeInsets.all(8.0),
                  child: Column(
                    children: [

                      Row(children: [
                        ServoControlSlider(initialValue: model.deviceData.servo5, onValueChanged: model.setServo5,),
                        ServoControlSlider(initialValue: model.deviceData.servo6, onValueChanged: model.setServo6,),
                      ],),

                      ServoControlSlider(initialValue: model.deviceData.servo4, onValueChanged: model.setServo4,),

                      ServoControlSlider(initialValue: model.deviceData.servo3, onValueChanged: model.setServo3,),

                      ServoControlSlider(initialValue: model.deviceData.servo2, onValueChanged: model.setServo2,),

                      ServoControlSlider(initialValue: model.deviceData.servo1, onValueChanged: model.setServo1,),

                    ],
                  ),
                ),
              ],
            ),
          ),
        ),
        // if (model.data!.d1)
        //   Positioned.fill(
        //       child: Warning(
        //     message: "Accident detected",
        //   )),
        // if (model.data2!.isSleeping)
        //   Positioned.fill(
        //       child: Warning(
        //     message: "Driver is sleeping",
        //   )),
      ],
    );
  }
}

class ConditionButton extends StatelessWidget {
  final String text;
  final bool isTrue;
  final VoidCallback onTap;

  const ConditionButton({
    required this.text,
    required this.isTrue,
    required this.onTap,
    Key? key,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(12.0),
      child: Container(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(20),
          color: isTrue ? Colors.red : Colors.green,
          boxShadow: const [
            BoxShadow(
              color: Colors.grey,
              offset: Offset(0.0, 1.0), //(x,y)
              blurRadius: 6.0,
            ),
          ],
        ),
        child: ClipRRect(
          clipBehavior: Clip.antiAlias,
          child: InkWell(
            onTap: onTap,
            child: Padding(
              padding: const EdgeInsets.all(20.0),
              child: Text(
                text,
                style: const TextStyle(
                  color: Colors.white,
                  fontWeight: FontWeight.w600,
                  fontSize: 20,
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }
}
class Warning extends StatelessWidget {
  final String message;
  const Warning({super.key, required this.message});

  @override
  Widget build(BuildContext context) {
    return BackdropFilter(
      filter: ImageFilter.blur(sigmaX: 10.0, sigmaY: 10.0),
      child: Center(
        child: Card(
          // elevation: 10,
          color: Colors.black.withOpacity(0.5),
          child: Container(
            height: 400,
            width: 250,
            child: Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  Lottie.network(
                      'https://assets2.lottiefiles.com/packages/lf20_Tkwjw8.json'),
                  const SizedBox(height: 20),
                  Text(
                    message,
                    textAlign: TextAlign.center,
                    style: const TextStyle(
                      fontSize: 15,
                      color: Colors.white,
                    ),
                  ),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}



class ServoControlSlider extends StatefulWidget {
  final int initialValue;
  final ValueChanged<int> onValueChanged;

  const ServoControlSlider({super.key,
    required this.initialValue,
    required this.onValueChanged,
  });

  @override
  _ServoControlSliderState createState() => _ServoControlSliderState();
}

class _ServoControlSliderState extends State<ServoControlSlider> {
  late int _sliderValue;

  @override
  void initState() {
    super.initState();
    _sliderValue = widget.initialValue;
    print(_sliderValue);
  }

  @override
  Widget build(BuildContext context) {
    return Slider(
      value: _sliderValue.toDouble(),
      min: 0,
      max: 180,
      divisions: 180,
      label: '$_sliderValue',
      onChanged: (double value) {
        setState(() {
          _sliderValue = value.round();
        });
      },
      onChangeEnd: (double value){
        widget.onValueChanged(value.round());
      },
    );
  }
}
