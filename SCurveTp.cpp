#include "SCurveTp.h"
#include <QtMath>

SCurveTp::SCurveTp(QObject *parent) : QObject(parent)
{
    // Начальные значения
    generateData(10.0, 1.0, 0.2); // Примерные начальные параметры
}

void SCurveTp::generateData(double maxSpeed, double acceleration, double jerk) {
    MotionData motionData = calculateMotion(maxSpeed, acceleration, jerk);

    m_trajectoryData.clear();
    m_velocityData.clear();
    m_accelerationData.clear();
    m_jerkData.clear();

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
/*
QList<QList<QPointF>> SCurveTp::calculateTrajectory(double maxSpeed, double acceleration, double jerk) {
    QList<QList<QPointF>> data;
    QList<QPointF> points;

    double totalTime = 100.0; // Примерное общее время движения
    double timeStep = 1.0; // Шаг по времени
    double currentSpeed = 0.0;
    double currentPosition = 0.0;
    double currentAcceleration = 0.0;
    double currentJerk = jerk;

    for (double t = 0; t <= totalTime; t += timeStep) {
        int phase = (int)(7 * t / totalTime); // Определение фазы движения

        switch (phase) {
        case 0: // Первая фаза разгона
        case 6: // Последняя фаза торможения
            currentAcceleration += currentJerk * timeStep;
            break;
        case 1: // Вторая фаза разгона
        case 5: // Вторая фаза торможения
            // Держим ускорение постоянным
            break;
        case 2: // Третья фаза разгона
        case 4: // Первая фаза торможения
            currentAcceleration -= currentJerk * timeStep;
            break;
        case 3: // Фаза движения с постоянной скоростью
            currentAcceleration = 0;
            break;
        }

        // Ограничение ускорения
        if (currentAcceleration > acceleration) currentAcceleration = acceleration;
        if (currentAcceleration < -acceleration) currentAcceleration = -acceleration;

        // Вычисление текущей скорости и позиции
        currentSpeed += currentAcceleration * timeStep;
        if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
        currentPosition += currentSpeed * timeStep;

        points.append(QPointF(t, currentPosition));
    }

    data.append(points);
    return data;
}

QList<QList<QPointF>> SCurveTp::calculateVelocity(double maxSpeed, double acceleration, double jerk) {
    QList<QList<QPointF>> data;
    QList<QPointF> points;
    double totalTime = 100.0;
    double timeStep = 1.0;
    double currentSpeed = 0.0;
    double currentAcceleration = 0.0;

    for (double t = 0; t <= totalTime; t += timeStep) {
        int phase = (int)(7 * t / totalTime);

        switch (phase) {
        case 0: // Первая фаза разгона
        case 6: // Последняя фаза торможения
            currentAcceleration += jerk * timeStep;
            break;
        case 1: // Вторая фаза разгона
        case 5: // Вторая фаза торможения
            break;
        case 2: // Третья фаза разгона
        case 4: // Первая фаза торможения
            currentAcceleration -= jerk * timeStep;
            break;
        case 3: // Фаза постоянной скорости
            currentAcceleration = 0;
            break;
        }

        if (currentAcceleration > acceleration) currentAcceleration = acceleration;
        if (currentAcceleration < -acceleration) currentAcceleration = -acceleration;

        currentSpeed += currentAcceleration * timeStep;
        if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
        if (currentSpeed < 0) currentSpeed = 0;

        points.append(QPointF(t, currentSpeed));
    }

    data.append(points);
    return data;
}

QList<QList<QPointF>> SCurveTp::calculateAcceleration(double maxSpeed, double acceleration, double jerk) {
    QList<QList<QPointF>> data;
    QList<QPointF> points;
    double totalTime = 100.0;
    double timeStep = 1.0;
    double currentAcceleration = 0.0;

    for (double t = 0; t <= totalTime; t += timeStep) {
        int phase = (int)(7 * t / totalTime);

        switch (phase) {
        case 0: // Первая фаза разгона
        case 6: // Последняя фаза торможения
            currentAcceleration += jerk * timeStep;
            break;
        case 1: // Вторая фаза разгона
        case 5: // Вторая фаза торможения
            break;
        case 2: // Третья фаза разгона
        case 4: // Первая фаза торможения
            currentAcceleration -= jerk * timeStep;
            break;
        case 3: // Фаза постоянной скорости
            currentAcceleration = 0;
            break;
        }

        if (currentAcceleration > acceleration) currentAcceleration = acceleration;
        if (currentAcceleration < -acceleration) currentAcceleration = -acceleration;

        points.append(QPointF(t, currentAcceleration));
    }

    data.append(points);
    return data;
}

QList<QList<QPointF>> SCurveTp::calculateJerk(double maxSpeed, double acceleration, double jerk) {
    QList<QList<QPointF>> data;
    QList<QPointF> points;
    double totalTime = 100.0;
    double timeStep = 1.0;
    double currentJerk = 0.0;

    for (double t = 0; t <= totalTime; t += timeStep) {
        int phase = (int)(7 * t / totalTime);

        switch (phase) {
        case 0: // Первая фаза разгона
        case 6: // Последняя фаза торможения
            currentJerk = jerk;
            break;
        case 1: // Вторая фаза разгона
        case 5: // Вторая фаза торможения
            currentJerk = 0;
            break;
        case 2: // Третья фаза разгона
        case 4: // Первая фаза торможения
            currentJerk = -jerk;
            break;
        case 3: // Фаза постоянной скорости
            currentJerk = 0;
            break;
        }

        points.append(QPointF(t, currentJerk));
    }

    data.append(points);
    return data;
}

*/

void SCurveTp::updateSeries(QAbstractSeries *series, const QString &name) {
    if (!series) return;

    QXYSeries *xySeries = static_cast<QXYSeries *>(series);

    // Сохранение ссылки на серию
    if (name == "Position") {
        m_trajectorySeries = xySeries;
        xySeries->replace(m_trajectoryData[0]); // Используйте первый набор данных
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
    double distance = 100.0; // Общая дистанция движения

    // Расчет базового времени ускорения и торможения (без джерка)
    double baseTimeAcceleration = maxSpeed / acceleration;
    double baseTimeDeceleration = baseTimeAcceleration;

    // Расчет времени джерка
    double jerkTime = baseTimeAcceleration * (jerkTimePercentage / 100.0);

    // Расчет общего времени ускорения и торможения с учетом джерка
    double totalTimeAcceleration = baseTimeAcceleration + jerkTime;
    double totalTimeDeceleration = totalTimeAcceleration;

    // Расчет времени и дистанции для фазы постоянной скорости при нулевом джерке
    double distanceAcceleration = 0.5 * acceleration * pow(baseTimeAcceleration, 2);
    double distanceDeceleration = distanceAcceleration;
    double distanceConstantSpeed = distance - (distanceAcceleration + distanceDeceleration);
    double timeConstantSpeed = distanceConstantSpeed / maxSpeed;

    // Общее время движения
    double totalTime = totalTimeAcceleration + totalTimeDeceleration + timeConstantSpeed;

    // Инициализация переменных для хранения текущего состояния
    double currentSpeed = 0.0, currentPosition = 0.0, currentAcceleration = 0.0, currentJerk = 0.0;
    double timeStep = 0.01; // Шаг времени





    for (double t = 0; t <= totalTime; t += timeStep) {
        // Определение текущей фазы движения
        if (t < jerkTime) {
            currentJerk = acceleration / jerkTime;
            currentAcceleration += currentJerk * timeStep;
        } else if (t < totalTimeAcceleration - jerkTime) {
            currentJerk = 0;
            currentAcceleration = acceleration;
        } else if (t < totalTimeAcceleration) {
            // Завершающая фаза разгона с джерком
            currentJerk = -acceleration / jerkTime;
            currentAcceleration += currentJerk * timeStep;
            // Плавное уменьшение ускорения
            if (currentAcceleration < 0) currentAcceleration = 0;
        } else if (t < totalTimeAcceleration + timeConstantSpeed) {
            currentJerk = 0;
            currentAcceleration = 0;
            currentSpeed = maxSpeed;
        } else if (t < totalTime - totalTimeDeceleration + jerkTime) {
            currentJerk = -acceleration / jerkTime;
            currentAcceleration += currentJerk * timeStep;
        } else if (t < totalTime - jerkTime) {
            currentJerk = 0;
            currentAcceleration = -acceleration;
        } else {
            currentJerk = acceleration / jerkTime;
            currentAcceleration += currentJerk * timeStep;
        }
        // Вычисление скорости и позиции
        currentSpeed += currentAcceleration * timeStep;
        currentPosition += currentSpeed * timeStep;

        // Ограничение скорости и позиции
        if (currentSpeed > maxSpeed) currentSpeed = maxSpeed;
        if (currentSpeed < 0) currentSpeed = 0;
        //if (currentPosition > distance) currentPosition = distance;

        // Добавление данных в списки
        data.trajectory.append(QPointF(t, currentPosition));
        data.velocity.append(QPointF(t, currentSpeed));
        data.acceleration.append(QPointF(t, currentAcceleration));
        data.jerk.append(QPointF(t, currentJerk));
    }
    double t=totalTime;
    // Если общее время меньше максимального времени, заполнить оставшееся время
    while (t < 100.0) {
        currentSpeed = (t >= totalTime) ? 0.0 : currentSpeed; // Скорость 0 после окончания движения
        currentAcceleration = (t >= totalTime) ? 0.0 : currentAcceleration; // Ускорение 0 после окончания движения
        currentJerk = (t >= totalTime) ? 0.0 : currentJerk; // Джерк 0 после окончания движения

        data.trajectory.append(QPointF(t, currentPosition));
        data.velocity.append(QPointF(t, currentSpeed));
        data.acceleration.append(QPointF(t, currentAcceleration));
        data.jerk.append(QPointF(t, currentJerk));

        t += timeStep;
    }
    return data;
}
