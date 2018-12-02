#ifndef __TT_POSITION_H__
#define __TT_POSITION_H__

#include <functional>

namespace TakeTen {
	class Position {
	public:
		Position();
		Position(const unsigned char c, const unsigned char r);
		Position(const Position& other);
		Position(Position&& other);
		~Position();

		unsigned char getColumn() const;
		unsigned char getRow() const;

		void setPosition(const unsigned char c, const unsigned char r);
		void setColumn(const unsigned char c);
		void setRow(const unsigned char r);

		Position& operator= (const Position& other) {
			if (this != &other) {
				_column = other._column;
				_row = other._row;
			}
			return *this;
		}

		Position& operator= (Position&& other) {
			if (this != &other) {
				_column = other._column;
				_row = other._row;
			}
			return *this;
		}

		bool operator ==(const Position& other) {
			bool result = true;
			if (this != &other) {
				result = _column == other._column && _row == other._row;
			}
			return result;
		}

		std::size_t getHash() const {
			size_t hash = (std::hash<int>()(_column) ^ std::hash<int>()(_row << 1)) >> 1;
			return hash;
		}

	private:

		unsigned char _column;
		unsigned char _row;

	};
}




#endif