#include "LooseOverlap.hpp"

using namespace std;

double LooseOverlap::Theta(const OverlapBlock& overlapBlock, const char& secondaryStructure)
{
    int overlapCount = OverlapLength(overlapBlock);
    if (secondaryStructure != 'C')
    {
        if (overlapCount >= ceil(overlapBlock.refRegion->GetLength() / 2.0))
            return 1;
        else
            return 0;
    }
    else
    {
        if (overlapCount >= 2)
            return 1;
        else
            return 0;
    }
}

LooseOverlap::LooseOverlap(const string& name, const string& refSequence, const string& predSequence, PrecalculatedMetric* precalculated) : Metric(refSequence, predSequence, precalculated) {
    this->name = name;
    for (auto& secondaryStructure : GetSecondaryStructureClasses()) {
        double summation = 0;
        int refLen = 0;
        for (auto& blockPtr : GetOverlappingBlocks(secondaryStructure)) {
            OverlapBlock overlapBlockPair = *blockPtr;
            summation += Theta(overlapBlockPair, secondaryStructure) * overlapBlockPair.refRegion->GetLength();
            refLen += overlapBlockPair.refRegion->GetLength();
        }
        this->refLengthSSMap.try_emplace(secondaryStructure, refLen);
        this->partialComputation.try_emplace(secondaryStructure, summation);
    }
}

double LooseOverlap::CalculateAllClasses() {
    double summation = 0;
    for (auto& secondaryStructure : GetSecondaryStructureClasses()) {
        summation += GetPartialComputation(secondaryStructure) / GetRefLength();
    }
    return summation;
}

double LooseOverlap::CalculateOneClass(const char& secondaryStructure) {
    return GetPartialComputation(secondaryStructure) / GetRefLength(secondaryStructure);
}