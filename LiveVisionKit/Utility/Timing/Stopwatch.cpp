//    Copyright (C) 2022  Sebastian Di Marco (crowsinc.dev@gmail.com)
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
// 	  **********************************************************************

#include "Stopwatch.hpp"

#include "Diagnostics/Directives.hpp"

namespace lvk
{

//---------------------------------------------------------------------------------------------------------------------

	Stopwatch::Stopwatch(const size_t history)
		: m_Running(false),
		  m_StartTime(),
		  m_Elapsed(),
		  m_History(history)
	{
		LVK_ASSERT(history > 0);
	}
	
//---------------------------------------------------------------------------------------------------------------------

	void Stopwatch::start()
	{
		m_Running = true;
		m_StartTime = Time::Now();
	}

//---------------------------------------------------------------------------------------------------------------------

	void Stopwatch::stop()
	{
		LVK_ASSERT(is_running());
		
		m_Elapsed = Time::Now() - m_StartTime;
		m_Running = false;
	
		m_History.push(m_Elapsed);
	}

//---------------------------------------------------------------------------------------------------------------------

	Time Stopwatch::restart()
	{
		// NOTE: we must ensure stop is run, or was ran, so 
		// that the elapsed time and history are properly set!
		if(is_running())
			stop();	

		start();
		return m_Elapsed;
	}

//---------------------------------------------------------------------------------------------------------------------

	bool Stopwatch::is_running() const
	{
		return m_Running;
	}

//---------------------------------------------------------------------------------------------------------------------

	Time Stopwatch::elapsed() const
	{
		return is_running() ? (Time::Now() - m_StartTime) : m_Elapsed;
	}

//---------------------------------------------------------------------------------------------------------------------

	Time Stopwatch::average() const
	{
		return m_History.is_empty() ? Time() : m_History.average();
	}

//---------------------------------------------------------------------------------------------------------------------

	Time Stopwatch::deviation() const
	{
		if(m_History.size() < 2)
			return Time(0);

		const Time average_time = m_History.average();

		Time total_deviation(0);
		for(auto i = 0; i < m_History.size(); i++)
		{
			const auto& current_time = m_History[i];
			if (average_time > current_time)
				total_deviation += average_time - current_time;
			else 
				total_deviation += current_time - average_time;
		}
		
		return total_deviation / static_cast<double>(m_History.size());
	}

//---------------------------------------------------------------------------------------------------------------------

	const SlidingBuffer<Time>& Stopwatch::history() const
	{
		return m_History;
	}

//---------------------------------------------------------------------------------------------------------------------

}