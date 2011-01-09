#ifndef DEFINES_H
#define DEFINES_H

#define PENALTY_TIME 20

namespace DJ {
	namespace Model {
		enum ProblemState {
			NOTSUBMITTED,
			SOLVED,
			FAILED,
			PENDING_SOLVED,
			PENDING_FAILED
		};
	}
}

#endif // DEFINES_H
