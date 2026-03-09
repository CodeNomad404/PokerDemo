#include "services/CardMatchService.h"

#include <cstdlib>

bool CardMatchService::canMatch(const CardModel& tableCard, const CardModel& topCard)
{
    if((tableCard.rank == CardFaceType::CFT_ACE && topCard.rank == CardFaceType::CFT_KING)||(tableCard.rank == CardFaceType::CFT_KING && topCard.rank == CardFaceType::CFT_ACE))
    {
        return true;
    }
    return std::abs(tableCard.rank - topCard.rank) == 1;
}
