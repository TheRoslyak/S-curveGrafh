#include "SCurveTp.h"
#include <QtMath>

SCurveTp::SCurveTp(QObject *parent) : QObject(parent)
{
    // Начальные значения
    //generateData(1500, 5735500.0, 100); // Примерные начальные параметры
    generateData(1500, 5735500.0, 100);
      //generateData(1472, 7420000.0, 100);
    //generateData(3000, 21500000, 100);
}

void SCurveTp::generateData(double maxSpeed, double acceleration, double jerk) {
    m_trajectoryData.clear();
    m_velocityData.clear();
    m_accelerationData.clear();
    m_jerkData.clear();
    MotionData motionData = calculateMotion(maxSpeed, acceleration, jerk);



    m_trajectoryData.append(motionData.trajectory);
    m_velocityData.append(motionData.velocity);
    m_accelerationData.append(motionData.acceleration);
    m_jerkData.append(motionData.jerk);

    // Обновление серий
    if (m_trajectorySeries) {
        updateSeries(m_trajectorySeries, "Position");
    }
    if (m_velocitySeries) {
        updateSeries(m_velocitySeries, "Velocity");
    }
    if (m_accelerationSeries) {
        updateSeries(m_accelerationSeries, "Acceleration");
    }
    if (m_jerkSeries) {
        updateSeries(m_jerkSeries, "Jerk");
    }
}


void SCurveTp::updateSeries(QAbstractSeries *series, const QString &name) {
    if (!series) return;

    QXYSeries *xySeries = static_cast<QXYSeries *>(series);

    // Сохранение ссылки на серию
    if (name == "Position") {
        m_trajectorySeries = xySeries;
        xySeries->replace(m_trajectoryData[0]);
    } else if (name == "Velocity") {
        m_velocitySeries = xySeries;
        xySeries->replace(m_velocityData[0]);
    } else if (name == "Acceleration") {
        m_accelerationSeries = xySeries;
        xySeries->replace(m_accelerationData[0]);
    } else if (name == "Jerk") {
        m_jerkSeries = xySeries;
        xySeries->replace(m_jerkData[0]);
    }
}

MotionData SCurveTp::calculateMotion(double maxSpeed, double acceleration, double jerkTimePercentage) {

        MotionData data;
        double distance = 180; // Общая дистанция движения


        double iGearbox=60;
        double iBearing=9.52;

        double gearRatio = iGearbox * iBearing;
        maxSpeed*=360/60;
        qDebug()<<maxSpeed;
        maxSpeed/=gearRatio;

        qDebug()<<maxSpeed;
        qDebug()<<acceleration;
        acceleration /= (gearRatio * gearRatio);
        qDebug()<<acceleration;

        double baseTimeAcceleration = maxSpeed / acceleration;
        double jerkTime = baseTimeAcceleration * (jerkTimePercentage / 100.0)/2;

        double distanceAcceleration = acceleration * pow(baseTimeAcceleration, 2) / 2;
        double distanceConstantSpeed = distance - 2 * distanceAcceleration;
        qDebug()<<"Угол на ускорение "<<distanceAcceleration/2;
        // Проверка, достаточно ли расстояния для постоянной скорости
        if (distanceConstantSpeed < 0) {
        maxSpeed = sqrt(distance * acceleration);
        baseTimeAcceleration = maxSpeed / acceleration;
        jerkTime = baseTimeAcceleration * (jerkTimePercentage / 100.0)/2;
        distanceConstantSpeed = 0;
        }

        double timeConstantSpeed = distanceConstantSpeed / maxSpeed;
        double totalTime = 2 * baseTimeAcceleration + timeConstantSpeed;

        double currentSpeed = 0.0, currentPosition = 0.0, currentAcceleration = 0.0, currentJerk = 0.0;
        double timeStep = 0.001;
        qDebug()<<jerkTime;
        qDebug()<<baseTimeAcceleration;
        qDebug()<<timeConstantSpeed;
        qDebug()<<totalTime;


        for (double t = 0; t <= totalTime; t += timeStep) {

            if (t < jerkTime) {
                currentJerk = acceleration/(jerkTime/2);
                currentAcceleration += currentJerk * timeStep;

                //currentAcceleration = acceleration + (acceleration * jerkTimePercentage / 100.0) ;
                //currentAcceleration += (acceleration * jerkTimePercentage / 100.0);

            } else if (t < baseTimeAcceleration - jerkTime) {
                currentJerk = 0;
                //currentAcceleration = acceleration ;
                currentAcceleration = acceleration * (1 + jerkTimePercentage / 100.0);
            } else if (t < baseTimeAcceleration) {

                currentJerk = -acceleration/(jerkTime/2);
                currentAcceleration += currentJerk * timeStep;

                //currentAcceleration = acceleration + (acceleration * jerkTimePercentage / 100.0);
                //currentAcceleration -= (acceleration * jerkTimePercentage / 100.0);

            } else if (t < baseTimeAcceleration + timeConstantSpeed)  {
                currentJerk = 0;
                currentAcceleration = 0;
                currentSpeed = maxSpeed;

            } else if (t < totalTime - baseTimeAcceleration + jerkTime) {
                currentJerk = -acceleration /(0.5*jerkTime);
                currentAcceleration += currentJerk * timeStep;
            } else if (t < totalTime - jerkTime) {
                currentJerk = 0;
                currentAcceleration = -acceleration ;
            } else {
                currentJerk = acceleration / (0.5*jerkTime);
                currentAcceleration += currentJerk * timeStep;
            }

            currentSpeed += currentAcceleration * timeStep;
            currentPosition += currentSpeed * timeStep;


            //if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
            //if (currentSpeed < 0) currentSpeed = 0;
            //if (currentPosition > distance) currentPosition = distance;


            double speed_rpm = currentSpeed * gearRatio * (60 / 360.0);

            data.trajectory.append(QPointF(t, currentPosition));
            data.velocity.append(QPointF(t, speed_rpm));
            data.acceleration.append(QPointF(t, currentAcceleration));
            data.jerk.append(QPointF(t, currentJerk));
        }
        qDebug()<<currentPosition;
        double t=totalTime;

        while (t < 100.0) {
            currentSpeed = (t >= totalTime) ? 0.0 : currentSpeed;
            currentAcceleration = (t >= totalTime) ? 0.0 : currentAcceleration;
            currentJerk = (t >= totalTime) ? 0.0 : currentJerk;
            data.trajectory.append(QPointF(t, currentPosition));
            data.velocity.append(QPointF(t, currentSpeed));
            data.acceleration.append(QPointF(t, currentAcceleration));
            data.jerk.append(QPointF(t, currentJerk));

            t += timeStep;
        }
        return data;
    }
