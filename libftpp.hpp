#ifndef LIBFTPP_HPP
#define LIBFTPP_HPP

/**
 * @file libftpp.hpp
 * @brief Unified include header for the libft++ toolbox.
 *
 * Including this file gives access to all modules of the libft++ library:
 * - Data structures
 * - Design patterns
 * - Networking
 * - Threading
 * - Mathematics
 * - Utility / Bonus tools
 */

// Data Structures
#include "data_structures/data_buffer/data_buffer.hpp"
#include "data_structures/data_structure.hpp"
#include "data_structures/pool/pool.hpp"

// Design Patterns
#include "design_patterns/design_patterns.hpp"
#include "design_patterns/memento/memento.hpp"
#include "design_patterns/observer/observer.hpp"
#include "design_patterns/singleton/singleton.hpp"
#include "design_patterns/state_machine/state_machine.hpp"

// Network
#include "network/client/client.hpp"
#include "network/message/message.hpp"
#include "network/server/server.hpp"

// Threading
#include "thread/persistent_worker/persistent_worker.hpp"
#include "thread/thread/thread.hpp"
#include "thread/thread_safe_iostream/thread_safe_iostream.hpp"
#include "thread/thread_safe_queue/thread_safe_queue.hpp"
#include "thread/threading.hpp"
#include "thread/worker_pool/worker_pool.hpp"

// Mathematic
#include "mathematics/IVector2/ivector2.hpp"
#include "mathematics/IVector3/ivector3.hpp"
#include "mathematics/perlin_noise_2D/perlin_noise_2D.hpp"
#include "mathematics/random_2D_coordinate_generator/random_2D_coordinate_generator.hpp"

// Bonus
#include "bonus/chronometre/chronometre.hpp"
#include "bonus/logger/logger.hpp"
#include "bonus/n_ary_tree/n_ary_tree.hpp"
#include "bonus/observable_value/IObserver/IObserver.hpp"
#include "bonus/observable_value/observable_value.hpp"
#include "bonus/ring_buffer/ring_buffer.hpp"

#endif