#ifndef SCURVETP_H
#define SCURVETP_H

#include <QtCore/QObject>
#include <QtCharts/QXYSeries>
#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtCharts/QAbstractSeries>


struct MotionData {
    QList<QPointF> trajectory;
    QList<QPointF> velocity;
    QList<QPointF> acceleration;
    QList<QPointF> jerk;
};
class SCurveTp : public QObject
{
    Q_OBJECT
public:
    explicit SCurveTp(QObject *parent = nullptr);

public slots:
    void generateData(double maxSpeed, double acceleration, double jerk);
    //void update(QAbstractSeries *series);
    void updateSeries(QAbstractSeries *series, const QString &name);

private:
    //QList<QList<QPointF>> calculateTrajectory(double maxSpeed, double acceleration, double jerk);
    //QList<QList<QPointF>> calculateVelocity(double maxSpeed, double acceleration, double jerk);
    //QList<QList<QPointF>> calculateAcceleration(double maxSpeed, double acceleration, double jerk);
    //QList<QList<QPointF>> calculateJerk(double maxSpeed, double acceleration, double jerk);

    MotionData calculateMotion(double maxSpeed, double acceleration, double jerk);

    QList<QList<QPointF>> m_trajectoryData;
    QList<QList<QPointF>> m_velocityData;
    QList<QList<QPointF>> m_accelerationData;
    QList<QList<QPointF>> m_jerkData;

    QAbstractSeries *m_trajectorySeries;
    QAbstractSeries *m_velocitySeries;
    QAbstractSeries *m_accelerationSeries;
    QAbstractSeries *m_jerkSeries;
};
#endif // SCURVETP_H
