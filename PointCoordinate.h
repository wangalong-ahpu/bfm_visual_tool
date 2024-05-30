#pragma once
//�ؼ���������
class PointCoordinate {
public:
    PointCoordinate();
    PointCoordinate(float x, float y);
    ~PointCoordinate();
    // ��ֵ�����
    PointCoordinate& operator=(const PointCoordinate& p);
    // �ӷ������
    PointCoordinate operator+(const PointCoordinate& p) const;
    // ��ֵ�����
    PointCoordinate operator/(int n) const;
    // ��ȡx��y��ֵ
    float get_x() const;
    float get_y() const;
    // ��ֵx��y��ֵ
    void input_x(const float& x);
    void input_y(const float& y);
private:
    float m_x, m_y;

};