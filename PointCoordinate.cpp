#include "PointCoordinate.h"
//关键点坐标类
PointCoordinate::PointCoordinate() {
    this->m_x = 0.0;
    this->m_y = 0.0;
};
PointCoordinate::PointCoordinate(float x, float y) : m_x(x), m_y(y) {}
PointCoordinate::~PointCoordinate() {}
// 赋值运算符
PointCoordinate& PointCoordinate::operator=(const PointCoordinate& p)
{
    if (this != &p) {
        this->m_x = p.m_x;
        this->m_y = p.m_y;
    }
    return *this;
}
// 加法运算符
PointCoordinate PointCoordinate::operator+(const PointCoordinate& p) const {
    return PointCoordinate(this->m_x + p.m_x, this->m_y + p.m_y);
}
// 均值运算符
PointCoordinate PointCoordinate::operator/(int n) const {
    return PointCoordinate(m_x / n, m_y / n);
}
// 获取x和y的值
float PointCoordinate::get_x() const {
    return m_x;
}
float PointCoordinate::get_y() const {
    return m_y;
}
// 赋值x和y的值
void PointCoordinate::input_x(const float& x) {
    this->m_x = x;
}
void PointCoordinate::input_y(const float& y) {
    this->m_y = y;
}