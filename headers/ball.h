#ifndef BALL_H

#define BALL_H

/**
 * @brief ball
 */
typedef struct Ball Ball;

/**
 * @brief The Ball struct
 */
struct Ball{
    double m_x;
    double m_y;
    double m_vx;
    double m_vy;
};

/**
 * @brief initializeBall : initialize ball
 * @param b : ball
 * @param x : position x
 * @param y : position y
 * @param vx : speed x
 * @param vy : speed y
 */
void initializeBall(Ball* b, double x, double y, double vx, double vy);

#endif
