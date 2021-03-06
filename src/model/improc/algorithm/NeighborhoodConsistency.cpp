#include "NeighborhoodConsistency.hpp"

#include <algorithm>
#include <numeric>

#include <utils/MathUtils.hpp>

namespace model {
namespace improc {
namespace algorithm {

NeighborhoodConsistency::NeighborhoodConsistency(const double consistencyThreshold, const uint32_t neighborhoodSize)
    : consistencyThreshold(consistencyThreshold)
    , neighborhoodSize(neighborhoodSize)
{
}

NeighborhoodConsistency::~NeighborhoodConsistency() = default;
NeighborhoodConsistency::NeighborhoodConsistency(const NeighborhoodConsistency&) = default;
NeighborhoodConsistency& NeighborhoodConsistency::operator=(const NeighborhoodConsistency&) = default;
NeighborhoodConsistency::NeighborhoodConsistency(NeighborhoodConsistency&&) = default;
NeighborhoodConsistency& NeighborhoodConsistency::operator=(NeighborhoodConsistency&&) = default;

MatchingPointsPairs NeighborhoodConsistency::filterConsistentPairs(const MatchingPointsPairs& matchingPointsPairs)
{
    std::vector<Point> aPoints;
    aPoints.reserve(matchingPointsPairs.size());
    std::vector<Point> bPoints;
    bPoints.reserve(matchingPointsPairs.size());

    std::for_each(matchingPointsPairs.cbegin(), matchingPointsPairs.cend(),
                  [&aPoints, &bPoints](const auto& pair)
                  {
                      aPoints.push_back(pair.first);
                      bPoints.push_back(pair.second);
                  });

    MatchingPointsPairs consistentPairs;
    for(const auto& pair : matchingPointsPairs)
    {
        std::vector<uint32_t> aPointNeighborhoodIndices = getPointNeighborhoodIndices(pair.first, aPoints);
        std::vector<uint32_t> bPointNeighborhoodIndices = getPointNeighborhoodIndices(pair.second, bPoints);
        std::vector<uint32_t> intersection;
        std::set_intersection(aPointNeighborhoodIndices.cbegin(), aPointNeighborhoodIndices.cend(),
                              bPointNeighborhoodIndices.cbegin(), bPointNeighborhoodIndices.cend(),
                              std::back_inserter(intersection));

        auto consistentPointsNum = intersection.size();
        if(static_cast<double>(consistentPointsNum) / neighborhoodSize >= consistencyThreshold)
            consistentPairs.push_back(pair);
    }

    return consistentPairs;
}

std::vector<uint32_t> NeighborhoodConsistency::getPointNeighborhoodIndices(const helpers::Point& point,
                                                                           const std::vector<helpers::Point>& allPoints)
{
    std::vector<uint32_t> indices(allPoints.size());
    std::iota(indices.begin(), indices.end(), 0u);

    std::sort(indices.begin(), indices.end(),
              [point, allPoints](const auto& lhs, const auto& rhs)
              {
                  return utils::math::euclideanDistance(point, allPoints[lhs])
                         < utils::math::euclideanDistance(point, allPoints[rhs]);
              });

    using difference_type = std::vector<int>::difference_type;
    std::vector<uint32_t> neighbors (indices.begin() + 1,
                                     indices.begin() + 1
                                         + static_cast<difference_type>(neighborhoodSize));

    std::sort(neighbors.begin(), neighbors.end());
    return neighbors;
}

} // namespace algorithm
} // namespace improc
} // namespace model
