#ifndef __TT_CELL_H__
#define __TT_CELL_H__

#include <functional>
#include "Position.h"

namespace TakeTen {
	class Cell {
	public:
		Cell();
		Cell(Cell&& other);
		Cell(const Cell& other);
		Cell(Position& position);
		Cell(Position& position, const char value);
		~Cell();

		const Position& getPosition() const;

		void reset();
		bool isZero() const;

		char getValue() const;
		void setValue(char value);

		Cell& operator= (const Cell& other) {
			if (this != &other) {
				_value = other._value;
				_position = other._position;
			}
			return *this;
		}

		Cell& operator= (Cell&& other) {
			if (this != &other) {
				_value = other._value;
				_position = other._position;
			}
			return *this;
		}

		inline bool sameValue(const Cell& other) {
			return _value == other._value || _value == 10 - other._value || 10 - _value == other._value;
		}

		bool operator ==(const Cell& other) {
			bool value = (_value == other._value) || (_value == 10 - other._value) || (10 - _value == other._value);
			bool position = _position == other._position;
			return value && position;
		}

		bool operator !=(const Cell& other) {
			return !(*this == other);
		}

		std::size_t getHash() const {
			size_t hash = (std::hash<int>()(_value) ^ (_position.getHash() << 1)) >> 1;
			return hash;
		}

	private:
		char _value;
		Position _position;
	};
}



#endif