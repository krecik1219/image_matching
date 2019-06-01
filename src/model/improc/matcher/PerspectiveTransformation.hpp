#pragma once

#include <utility>
#include <vector>

#include <Eigen/Dense>

#include <helpers/Point.hpp>


namespace model {
namespace improc {
namespace matcher {

using helpers::Point;
using PointsPair = std::pair<Point, Point>;

using Matrix8d = Eigen::Matrix<double, 8, 8>;
using Vector8d = Eigen::Matrix<double, 8, 1>;
using RowVector8d = Eigen::Matrix<double, 1, 8>;

class PerspectiveTransformation
{
public:
    PerspectiveTransformation();
    PerspectiveTransformation(
        const PointsPair& pair1, const PointsPair& pair2, const PointsPair& pair3, const PointsPair& parir4);

    ~PerspectiveTransformation();

    PerspectiveTransformation(const PerspectiveTransformation&) = delete;
    PerspectiveTransformation& operator=(const PerspectiveTransformation&) = delete;
    PerspectiveTransformation(PerspectiveTransformation&&) = delete;
    PerspectiveTransformation& operator=(PerspectiveTransformation&&) = delete;
    Point applyTransformation(const Point& point) const;
    Eigen::Matrix2Xd applyMassiveTransformation(
        const Eigen::Matrix2Xd& points) const;

    void buildModel(const PointsPair& pair1,
                    const PointsPair& pair2,
                    const PointsPair& pair3,
                    const PointsPair& pair4);

private:
    Eigen::Matrix3d calculateModel(const PointsPair& pair1,
                                   const PointsPair& pair2,
                                   const PointsPair& pair3,
                                   const PointsPair& pair4) const;

    Eigen::Matrix3d transformationMatrix;
};

} // namespace matcher
} // namespace improc
} // namespace model
