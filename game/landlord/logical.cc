//! Copyright (c) 2014 ASMlover. All rights reserved.
//!
//! Redistribution and use in source and binary forms, with or without
//! modification, are permitted provided that the following conditions
//! are met:
//!
//!  * Redistributions of source code must retain the above copyright
//!    notice, this list ofconditions and the following disclaimer.
//!
//!  * Redistributions in binary form must reproduce the above copyright
//!    notice, this list of conditions and the following disclaimer in
//!    the documentation and/or other materialsprovided with the
//!    distribution.
//!
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//! "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//! LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//! FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//! COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//! INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//! BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//! LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//! CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//! LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//! ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//! POSSIBILITY OF SUCH DAMAGE.
#include "global.h"
#include "logical.h"


#define CARDVALUE_SMALL_KING  (13)
#define CARDVALUE_BIG_KING    (14)
#define CARDVALUE_2POINT      (12)


Logical::Logical(void) {
}

Logical::~Logical(void) {
}

bool Logical::PlayAnyCard(
    const std::vector<uint8_t>& cards, std::vector<uint8_t>& out_cards) {
  std::map<uint8_t, std::vector<Card> > cards_map;
  if (!CardsAnalysis(cards, cards_map))
    return false;

  out_cards.clear();
  return PlayStraight(cards_map, out_cards) 
    || PlayAnySingle(out_cards) 
    || PlayAnyPair(out_cards) 
    || PlayAnyThree(out_cards) 
    || PlayAnyThreeWithSingle(out_cards) 
    || PlayAnyThreeWithPair(out_cards) 
    || PlayAnyBomb(out_cards) 
    || PlayRocket(out_cards);
}

bool Logical::PlayCard(CardType type, uint8_t value, 
    const std::vector<uint8_t>& cards, std::vector<uint8_t>& out_cards) {
  std::map<uint8_t, std::vector<Card> > cards_map;
  if (!CardsAnalysis(cards, cards_map))
    return false;
  
  out_cards.clear();
  switch (type) {
  case CARDTYPE_SINGLE:
    return PlaySingle(value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_PAIR:
    return PlayPair(value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_THREE:
    return PlayThree(value, out_cards) 
      || PlayAnyBomb(out_cards)
      || PlayRocket(out_cards);
  case CARDTYPE_THREE_WITH_SINGLE:
    return PlayThreeWithSingle(value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_THREE_WITH_PAIR:
    return PlayThreeWithPair(value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_BOMB:
    return PlayBomb(value, out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_FOUR_WITH_TWOSINGLE:
    return PlayFourWithTwoSingle(value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_FOUR_WITH_TWOPAIR:
    return PlayFourWithTwoPair(value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_STRAIGHT_SINGLE5:
  case CARDTYPE_STRAIGHT_SINGLE6:
  case CARDTYPE_STRAIGHT_SINGLE7:
  case CARDTYPE_STRAIGHT_SINGLE8:
  case CARDTYPE_STRAIGHT_SINGLE9:
  case CARDTYPE_STRAIGHT_SINGLE10:
  case CARDTYPE_STRAIGHT_SINGLE11:
  case CARDTYPE_STRAIGHT_SINGLE12:
    return PlayStraightSingle(
        5 + type - CARDTYPE_STRAIGHT_SINGLE5, value, out_cards) 
      || PlayAnyBomb(out_cards)
      || PlayRocket(out_cards);
  case CARDTYPE_STRAIGHT_PAIR3:
  case CARDTYPE_STRAIGHT_PAIR4:
  case CARDTYPE_STRAIGHT_PAIR5:
  case CARDTYPE_STRAIGHT_PAIR6:
  case CARDTYPE_STRAIGHT_PAIR7:
  case CARDTYPE_STRAIGHT_PAIR8:
  case CARDTYPE_STRAIGHT_PAIR9:
  case CARDTYPE_STRAIGHT_PAIR10:
    return PlayStraightPair(
        3 + type - CARDTYPE_STRAIGHT_PAIR3, value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_STRAIGHT_THREE2:
  case CARDTYPE_STRAIGHT_THREE3:
  case CARDTYPE_STRAIGHT_THREE4:
  case CARDTYPE_STRAIGHT_THREE5:
  case CARDTYPE_STRAIGHT_THREE6:
  case CARDTYPE_STRAIGHT_THREE7:
    return PlayStraightThree(
        2 + type - CARDTYPE_STRAIGHT_THREE2, value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_AIRPLANE_SINGLE2:
  case CARDTYPE_AIRPLANE_SINGLE3:
  case CARDTYPE_AIRPLANE_SINGLE4:
  case CARDTYPE_AIRPLANE_SINGLE5:
    return PlayAirplaneWithSingle(
        2 + type - CARDTYPE_AIRPLANE_SINGLE2, value, out_cards) 
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_AIRPLANE_PAIR2:
  case CARDTYPE_AIRPLANE_PAIR3:
  case CARDTYPE_AIRPLANE_PAIR4:
    return PlayAirplaneWithPair(
        2 + type - CARDTYPE_AIRPLANE_PAIR2, value, out_cards)
      || PlayAnyBomb(out_cards) 
      || PlayRocket(out_cards);
  case CARDTYPE_UNKNOWN:
  default:
    return false;
  }

  return false;
}




bool Logical::CardsAnalysis(
    const std::vector<uint8_t>& cards, 
    std::map<uint8_t, std::vector<Card> >& cards_map) {
  if (cards.empty())
    return false;

  // card value => cards 
  cards_map.clear();
  Card c;
  int  n = static_cast<int>(cards.size());
  for (int i = 0; i < n; ++i) {
    c.card = cards[i];
    c.value = CardValue(cards[i]);

    cards_map[c.value].push_back(c);
  }

  std::map<uint8_t, std::vector<Card> >::iterator it;
  Card joker = {0};
  for (it = cards_map.begin(); it != cards_map.end(); ++it) {
    switch (it->second.size()) {
    case 1:
      if (CARDVALUE_SMALL_KING == it->second[0].value) {
        joker = it->second[0];
      }
      else if (CARDVALUE_BIG_KING == it->second[0].value 
          && joker.value != 0) {
        rocket_.push_back(joker);
        rocket_.push_back(it->second[0]);
      }
      else {
        single_.push_back(it->second[0]);
      }
      break;
    case 2:
      pair_.push_back(it->second[0]);
      pair_.push_back(it->second[1]);
      break;
    case 3:
      three_.push_back(it->second[0]);
      three_.push_back(it->second[1]);
      three_.push_back(it->second[2]);
      break;
    case 4:
      bomb_.push_back(it->second[0]);
      bomb_.push_back(it->second[1]);
      bomb_.push_back(it->second[2]);
      bomb_.push_back(it->second[3]);
      break;
    default:
      return false;
    }
  }

  return true;
}

bool Logical::IsContinued(const Card* cards, int count, int step) {
  for (int i = 0; i < count - step; i += step) {
    if (cards[i].value + 1 != cards[i + step].value 
        || CARDVALUE_2POINT == cards[i + step].value)
      return false;
  }

  return true;
}

bool Logical:: IsContinued(
      const std::pair<uint8_t, std::vector<Card> >* cards, 
      int count) {
  for (int i = 0; i < count - 1; ++i) {
    if (cards[i].first + 1 != cards[i + 1].first
        || CARDVALUE_2POINT == cards[i + 1].first)
      return false;
  }

  return true;
}


bool Logical::PlayAnySingle(std::vector<uint8_t>& out_cards) {
  if (single_.empty())
    return false;

  out_cards.push_back(single_[0].card);

  return true;
}

bool Logical::PlayAnyPair(std::vector<uint8_t>& out_cards) {
  if (pair_.size() < 2)
    return false;

  out_cards.push_back(pair_[0].card);
  out_cards.push_back(pair_[1].card);

  return true;
}

bool Logical::PlayAnyThree(std::vector<uint8_t>& out_cards) {
  if (three_.size() < 3)
    return false;

  out_cards.push_back(three_[0].card);
  out_cards.push_back(three_[1].card);
  out_cards.push_back(three_[2].card);

  return true;
}

bool Logical::PlayAnyThreeWithSingle(std::vector<uint8_t>& out_cards) {
  if (single_.empty() || three_.size() < 3)
    return false;

  out_cards.push_back(three_[0].card);
  out_cards.push_back(three_[1].card);
  out_cards.push_back(three_[2].card);
  out_cards.push_back(single_[0].card);

  return true;
}

bool Logical::PlayAnyThreeWithPair(std::vector<uint8_t>& out_cards) {
  if (pair_.size() < 2 || three_.size() < 3)
    return false;
  
  out_cards.push_back(three_[0].card);
  out_cards.push_back(three_[1].card);
  out_cards.push_back(three_[2].card);
  out_cards.push_back(pair_[0].card);
  out_cards.push_back(pair_[1].card);

  return true;
}

bool Logical::PlayAnyBomb(std::vector<uint8_t>& out_cards) {
  if (bomb_.size() < 4)
    return false;

  out_cards.push_back(bomb_[0].card);
  out_cards.push_back(bomb_[1].card);
  out_cards.push_back(bomb_[2].card);
  out_cards.push_back(bomb_[3].card);

  return true;
}

bool Logical::PlayRocket(std::vector<uint8_t>& out_cards) {
  if (rocket_.size() < 2)
    return false;

  out_cards.push_back(rocket_[0].card);
  out_cards.push_back(rocket_[1].card);

  return true;
}

bool Logical::PlayStraight(
    std::map<uint8_t, std::vector<Card> >& cards, 
    std::vector<uint8_t>& out_cards) {
  if (cards.size() < 5)
    return false;

  std::map<uint8_t, std::vector<Card> >::iterator it;
  std::vector<std::pair<uint8_t, std::vector<Card> > > temp_cards;
  for (it = cards.begin(); it != cards.end(); ++it) {
    temp_cards.push_back(
        std::make_pair<uint8_t, std::vector<Card> >(
          it->first, it->second));
  }

  int n = static_cast<int>(temp_cards.size());
  for (int i = 0; i < n && (n - 1 >= 5); ++i) {
    if (IsContinued(&temp_cards[i])) {
      for (int j = i; j < 5; ++j)
        out_cards.push_back(temp_cards[j].second[0].card);

      return true;
    }
  }

  return false;
}



bool Logical::PlaySingle(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  int n = static_cast<int>(single_.size());
  for (int i = 0; i < n; ++i) {
    if (single_[i].value > value) {
      out_cards.push_back(single_[i].card);
      return true;
    }
  }

  return false;
}

bool Logical::PlayPair(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  int n = static_cast<int>(pair_.size());
  for (int i = 0; i < n; i += 2) {
    if (pair_[i].value > value && (i + 1 < n)) {
      out_cards.push_back(pair_[i].card);
      out_cards.push_back(pair_[i + 1].card);
      return true;
    }
  }

  return false;
}

bool Logical::PlayThree(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  int n = static_cast<int>(three_.size());
  for (int i = 0; i < n; i += 3) {
    if (three_[i].value > value && (i + 2 < n)) {
      out_cards.push_back(three_[i].card);
      out_cards.push_back(three_[i + 1].card);
      out_cards.push_back(three_[i + 2].card);
      return true;
    }
  }

  return false;
}

bool Logical::PlayThreeWithSingle(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  if (single_.empty() || !PlayThree(value, out_cards))
    return false;

  out_cards.push_back(single_[0].card);

  return true;
}

bool Logical::PlayThreeWithPair(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  if (pair_.size() < 2 || !PlayThree(value, out_cards))
    return false;

  out_cards.push_back(pair_[0].card);
  out_cards.push_back(pair_[1].card);

  return true;
}

bool Logical::PlayBomb(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  int n = static_cast<int>(bomb_.size());
  for (int i = 0; i < n; i += 4) {
    if (bomb_[i].value > value && (i + 3 < n)) {
      out_cards.push_back(bomb_[i].card);
      out_cards.push_back(bomb_[i + 1].card);
      out_cards.push_back(bomb_[i + 2].card);
      out_cards.push_back(bomb_[i + 3].card);

      return true;
    }
  }

  return false;
}

bool Logical::PlayFourWithTwoSingle(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  if (single_.size() < 2 || !PlayBomb(value, out_cards))
    return false;

  out_cards.push_back(single_[0].card);
  out_cards.push_back(single_[1].card);

  return true;
}

bool Logical::PlayFourWithTwoPair(
    uint8_t value, std::vector<uint8_t>& out_cards) {
  if (pair_.size() < 4 || !PlayBomb(value, out_cards))
    return false;

  out_cards.push_back(pair_[0].card);
  out_cards.push_back(pair_[1].card);
  out_cards.push_back(pair_[2].card);
  out_cards.push_back(pair_[3].card);

  return true;
}

bool Logical::PlayStraightSingle(int num, 
    uint8_t value, std::vector<uint8_t>& out_cards) {
  int n = static_cast<int>(single_.size());
  if (n < num)
    return false;

  for (int i = n - 1; i >= 0; --i) {
    if (single_[i].value > value && i >= (num - 1)) {
      if (IsContinued(&single_[i - num + 1], num)) {
        for (int j = i + 1 - num; j < num; ++j)
          out_cards.push_back(single_[j].card);

        return true;
      }
    }
  }

  return false;
}

bool Logical::PlayStraightPair(int num, 
    uint8_t value, std::vector<uint8_t>& out_cards) {
  int n = static_cast<int>(pair_.size());
  if (n < num * 2)
    return false;

  for (int i = n - 1; i >= 0; i -= 2) {
    if (pair_[i].value > value && ((i - 1) / 2 >= (num - 1))) {
      if (IsContinued(&pair_[i + 1 - num * 2], num * 2, 2)) {
        for (int j = i + 1 - num * 2; j < num * 2; ++j)
          out_cards.push_back(pair_[j].card);

        return true;
      }
    }
  }

  return false;
}

bool Logical::PlayStraightThree(int num, 
    uint8_t value, std::vector<uint8_t>& out_cards) {
  int n = static_cast<int>(three_.size());
  if (n < num * 3)
    return false;

  for (int i = n - 1; i >= 0; i -= 3) {
    if (three_[i].value > value && ((i - 1) / 3 >= (num - 1))) {
      if (IsContinued(&three_[i + 1 - num * 3], num * 3, 3)) {
        for (int j = i + 1 - num * 3; j < num * 3; ++j)
          out_cards.push_back(three_[j].card);

        return true;
      }
    }
  }

  return false;
}

bool Logical::PlayAirplaneWithSingle(int num, 
    uint8_t value, std::vector<uint8_t>& out_cards) {
  if (single_.size() < 2 || !PlayStraightThree(num, value, out_cards))
    return false;

  out_cards.push_back(single_[0].card);
  out_cards.push_back(single_[1].card);

  return true;
}

bool Logical::PlayAirplaneWithPair(int num, 
    uint8_t value, std::vector<uint8_t>& out_cards) {
  if (pair_.size() < 4 || !PlayStraightThree(num, value, out_cards))
    return false;

  out_cards.push_back(pair_[0].card);
  out_cards.push_back(pair_[1].card);
  out_cards.push_back(pair_[2].card);
  out_cards.push_back(pair_[3].card);

  return true;
}
