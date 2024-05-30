#pragma once
//关键点坐标类
class PointCoordinate {
public:
    PointCoordinate();
    PointCoordinate(float x, float y);
    ~PointCoordinate();
    // 赋值运算符
    PointCoordinate& operator=(const PointCoordinate& p);
    // 加法运算符
    PointCoordinate operator+(const PointCoordinate& p) const;
    // 均值运算符
    PointCoordinate operator/(int n) const;
    // 获取x和y的值
    float get_x() const;
    float get_y() const;
    // 赋值x和y的值
    void input_x(const float& x);
    void input_y(const float& y);
private:
    float m_x, m_y;

};