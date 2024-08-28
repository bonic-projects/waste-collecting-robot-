import 'package:stacked/stacked.dart';
import 'package:boa_flutter/app/app.locator.dart';
import 'package:boa_flutter/app/app.router.dart';
import 'package:stacked_services/stacked_services.dart';

import '../../../services/database_service.dart';

class StartupViewModel extends BaseViewModel {
  final _navigationService = locator<NavigationService>();
  final _databaseService = locator<DatabaseService>();

  // Place anything here that needs to happen before we get into the application
  Future runStartupLogic() async {
    _databaseService.setupNodeListening();
    await Future.delayed(const Duration(seconds: 3));

    // This is where you can make decisions on where your app should navigate when
    // you have custom startup logic

    _navigationService.replaceWithHomeView();
  }
}
