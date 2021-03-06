#pragma once

#include <cstdint>

#include "RansacAlgo.hpp"
#include <improc/algorithm/transformation/TransformationModelPerspective.hpp>

namespace model {
namespace improc {
namespace algorithm {

using helpers::Point;
using MatchingPointsPairs = std::vector<std::pair<Point, Point>>;

class Ransac
{
public:
    Ransac();
    virtual ~Ransac();

    Ransac(const Ransac&) = delete;
    Ransac& operator=(const Ransac&) = delete;
    Ransac(Ransac&&) = delete;
    Ransac& operator=(Ransac&&) = delete;

    virtual std::pair<std::unique_ptr<TransformationModel>, MatrixData>
        runRansac(const MatchingPointsPairs& pointsPairs) = 0;

    virtual uint32_t getItersNum() const = 0;

    template<class Model, template<class, class, class> class RansacAlgoDecorator, class... Args>
    static void decorate(
        std::unique_ptr<RansacAlgo<Model, MatrixData, typename MatrixData::Sample>>& algoBase,
        Args... args);

protected:
    template<class Model, template<class, class, class> class RansacAlgo>
    static std::pair<std::unique_ptr<TransformationModel>, MatrixData> run(
            RansacAlgo<Model, MatrixData, typename MatrixData::Sample>& algo, const MatchingPointsPairs& pointsPairs);
};

template<class Model, template<class, class, class> class RansacAlgoDecorator, class... Args>
void Ransac::decorate(
    std::unique_ptr<RansacAlgo<Model, MatrixData, typename MatrixData::Sample>>& algoBase,
    Args... args)
{
    algoBase =
        std::make_unique<RansacAlgoDecorator<Model, MatrixData, typename MatrixData::Sample>>(
            std::move(algoBase), std::forward<Args>(args)...);
}

template<class Model, template<class, class, class> class RansacAlgo>
std::pair<std::unique_ptr<TransformationModel>, MatrixData> Ransac::run(
    RansacAlgo<Model, MatrixData, typename MatrixData::Sample>& algo, const MatchingPointsPairs& pointsPairs)
{
    MatrixData data(pointsPairs);
    return algo.runRansac(data);
}

} // namespace algorithm
} // namespace improc
} // namespace model
