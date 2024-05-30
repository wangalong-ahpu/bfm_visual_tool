#include "PointCoordinate.h"
//�ؼ���������
PointCoordinate::PointCoordinate() {
    this->m_x = 0.0;
    this->m_y = 0.0;
};
PointCoordinate::PointCoordinate(float x, float y) : m_x(x), m_y(y) {}
PointCoordinate::~PointCoordinate() {}
// ��ֵ�����
PointCoordinate& PointCoordinate::operator=(const PointCoordinate& p)
{
    if (this != &p) {
        this->m_x = p.m_x;
        this->m_y = p.m_y;
    }
    return *this;
}
// �ӷ������
PointCoordinate PointCoordinate::operator+(const PointCoordinate& p) const {
    return PointCoordinate(this->m_x + p.m_x, this->m_y + p.m_y);
}
// ��ֵ�����
PointCoordinate PointCoordinate::operator/(int n) const {
    return PointCoordinate(m_x / n, m_y / n);
}
// ��ȡx��y��ֵ
float PointCoordinate::get_x() const {
    return m_x;
}
float PointCoordinate::get_y() const {
    return m_y;
}
// ��ֵx��y��ֵ
void PointCoordinate::input_x(const float& x) {
    this->m_x = x;
}
void PointCoordinate::input_y(const float& y) {
    this->m_y = y;
}