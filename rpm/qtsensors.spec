Name:       qt5-qtsensors
Summary:    Qt Sensors module
Version:    5.6.3
Release:    1
License:    (LGPLv2 or LGPLv3) with exception or GPLv3 or Qt Commercial
URL:        https://github.com/qt/qtsensors
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  qt5-qtcore-devel
BuildRequires:  qt5-qtgui-devel
BuildRequires:  qt5-qtopengl-devel
BuildRequires:  qt5-qtnetwork-devel
BuildRequires:  qt5-qtdeclarative-qtquick-devel
BuildRequires:  qt5-qmake
BuildRequires:  fdupes
BuildRequires:  pkgconfig(sensord-qt5) >= 0.13.0

%description
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt sensors module

%package devel
Summary:    Qt sensors - development files
Requires:   %{name} = %{version}-%{release}

%description devel
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.
.
This package contains the Qt sensors module development files

%package -n qt5-qtdeclarative-import-sensors
Summary:    QtQml sensors import
Requires:   %{name} = %{version}-%{release}
Requires:   qt5-qtdeclarative

%description -n qt5-qtdeclarative-import-sensors
This package contains the Sensors import for Qtml

%package plugin-sensorfw
Summary:    sensorfw sensors plugin
Requires:   %{name} = %{version}-%{release}
Requires:   sensorfw-qt5 >= 0.13.0

%description plugin-sensorfw
This package contains the sensorfw plugin for sensors

%package plugin-generic
Summary:    Generic sensors plugin
Requires:   %{name} = %{version}-%{release}

%description plugin-generic
This package contains the generic plugin for sensors

%package plugin-gestures-shake
Summary:    Shake gesture plugin
Requires:   %{name} = %{version}-%{release}

%description plugin-gestures-shake
This package contains the shake gesture plugin for sensors

%package plugin-gestures-sensor
Summary:    Sensor gesture plugin
Requires:   %{name} = %{version}-%{release}

%description plugin-gestures-sensor
This package contains the gesture plugin for sensors


%prep
%setup -q -n %{name}-%{version}

%build
touch .git
%qmake5 CONFIG+=sensorfw
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install
# Remove unneeded .la files
rm -f %{buildroot}/%{_libdir}/*.la
# Remove unneeded cmake file
rm -f %{buildroot}/%{_libdir}/cmake/Qt5Sensors/Qt5Sensors_genericSensorPlugin.cmake
rm -f %{buildroot}/%{_libdir}/cmake/Qt5Sensors/Qt5Sensors_QShakeSensorGesturePlugin.cmake
rm -f %{buildroot}/%{_libdir}/cmake/Qt5Sensors/Qt5Sensors_QtSensorGesturePlugin.cmake
rm -f %{buildroot}/%{_libdir}/cmake/Qt5Sensors/Qt5Sensors_sensorfwSensorPlugin.cmake
# Remove default sensor configuration which comes from adaptation
rm -f %{buildroot}/%{_sysconfdir}/xdg/QtProject/Sensors.conf
%fdupes %{buildroot}/%{_includedir}


%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%license LICENSE.LGPLv* LGPL_EXCEPTION.txt
%license LICENSE.GPLv3 LICENSE.FDL
%{_qt5_libdir}/libQt5Sensors.so.5
%{_qt5_libdir}/libQt5Sensors.so.5.*

%files devel
%defattr(-,root,root,-)
%{_qt5_libdir}/libQt5Sensors.so
%{_qt5_libdir}/libQt5Sensors.prl
%{_qt5_libdir}/pkgconfig/*
%{_qt5_includedir}/*
%{_qt5_archdatadir}/mkspecs/
%{_qt5_libdir}/cmake/

%files -n qt5-qtdeclarative-import-sensors
%defattr(-,root,root,-)
%{_qt5_archdatadir}/qml/QtSensors/

%files plugin-sensorfw
%defattr(-,root,root,-)
%{_qt5_plugindir}/sensors/libqtsensors_sensorfw.so

%files plugin-generic
%defattr(-,root,root,-)
%{_qt5_plugindir}/sensors/libqtsensors_generic.so

%files plugin-gestures-shake
%defattr(-,root,root,-)
%{_qt5_plugindir}/sensorgestures/libqtsensorgestures_shakeplugin.so

%files plugin-gestures-sensor
%defattr(-,root,root,-)
%{_qt5_plugindir}/sensorgestures/libqtsensorgestures_plugin.so
