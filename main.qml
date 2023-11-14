import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import com.company.scurvetp 1.0
Window {
    width: 1240
    height: 1024
    visible: true
    SCurveTp{
        id:myModel


    Component.onCompleted: {
        console.log("Initializing model Data");
        myModel.updateSeries(lineSeries1, "Position");
        myModel.updateSeries(lineSeries2, "Velocity");
        myModel.updateSeries(lineSeries3, "Acceleration");
        myModel.updateSeries(lineSeries4, "Jerk");
        console.log("Model initialized Data");
    }
    }
    title: qsTr("Hello World")
        id: rec
        ColumnLayout {
            anchors.fill: parent
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            spacing: 0

               GroupBox {
                   id: groupBox
                   //title: "Charts"
                   height: 900
                   Layout.fillWidth: true
                   Layout.preferredHeight:900


                   ColumnLayout {
                       anchors.fill: parent
                       spacing: 0
                       //anchors.margins: 10
                   ChartView {
                       //width: 1024
                       Layout.preferredHeight: groupBox.height/3
                       anchors.left: parent.left
                       anchors.right: parent.right
                       anchors.rightMargin: 0
                       anchors.leftMargin: 0
                       legend.visible: false

                               LineSeries {
                                   id:lineSeries1
                                   name: "Position"
                                   axisXTop:  axisTime
                                   axisY:  axisPosition

                               }


                               ValueAxis {
                                   id: axisPosition
                                   titleText: "Position (m)"
                                   labelFormat: "%.2f"
                                   min: 0
                                   max: 150
                               }
                               ValueAxis {
                                   id: axisTime
                                   //titleText: "Time (s)"
                                   //labelFormat: "%.1f"
                                  // tickCount: 10
                                   min: 0
                                   max: 10
                               }

                           }

                   ChartView {

                               height: groupBox.height/4
                               anchors.left: parent.left
                               anchors.right: parent.right
                               anchors.rightMargin: 0
                               anchors.leftMargin: 0
                               legend.visible: false
                               LineSeries {
                                   id:lineSeries2
                                   name: "Velocity"
                                   color: "#20df2d"
                                   axisX: axisTime2
                                   axisY: axisVelocity

                               }


                               ValueAxis {
                                   id:  axisVelocity
                                   titleText: "Velocity"
                                   labelFormat: "%.2f"
                                   min: 0
                                   max: 60
                               }
                               ValueAxis {
                                   id: axisTime2
                                   min: 0
                                   max: 10
                               }
                           }



                   ChartView {

                height: groupBox.height/4
                               anchors.left: parent.left
                               anchors.right: parent.right
                               anchors.rightMargin: 0
                               anchors.leftMargin: 0
                               legend.visible: false
                               LineSeries {
                                   id:lineSeries3
                                   axisX: axisTime3
                                   axisY: axisAcceleration

                                   color: "#d6df20"

                               }

                               ValueAxis {
                                   id: axisAcceleration
                                   titleText: "Acceleration"
                                   labelFormat: "%.2f"
                                   min: -100
                                   max: 100

                               }
                               ValueAxis {
                                   id: axisTime3
                                   min: 0
                                   max: 10
                               }
                           }


                   ChartView {

                               height: groupBox.height/5
                               anchors.left: parent.left
                               anchors.right: parent.right
                               anchors.rightMargin: 0
                               anchors.leftMargin: 0
                               legend.visible: false
                               LineSeries {
                                   id:lineSeries4

                                   color: "#df2020"
                                   axisX: axisTime4
                                   axisY: axisJerk

                               }

                               ValueAxis {
                                   id: axisJerk
                                   titleText: "Jerk"
                                   min: -100
                                   max: 100
                               }
                               ValueAxis {
                                   id: axisTime4
                                   min: 0
                                   max: 10
                               }
                           }


               }

    }
               GroupBox {

                   Layout.fillWidth: true

                   ColumnLayout {
                       spacing: 10
                       width: parent.width

                       // Слайдер для максимальной скорости
                       Slider {
                           id: maxVelocitySlider
                           from: 0
                           to: 100
                           Layout.fillWidth: true
                           stepSize: 1
                           onValueChanged: myModel.generateData(maxVelocitySlider.value,maxAccelerationSlider.value,jerkSlider.value)
                       }

                       // Слайдер для максимального ускорения
                       Slider {
                           id: maxAccelerationSlider
                           from: 0
                           to: 100
                           Layout.fillWidth: true
                           stepSize: 1
                           onValueChanged: myModel.generateData(maxVelocitySlider.value,maxAccelerationSlider.value,jerkSlider.value)
                       }


    // Слайдер для jerk
                       Slider {
                           id: jerkSlider
                           from: 0
                           to: 100
                           Layout.fillWidth: true
                           stepSize: 1
                           onValueChanged: myModel.generateData(maxVelocitySlider.value,maxAccelerationSlider.value,jerkSlider.value)

                       }

                   }
               }

           }
    }

