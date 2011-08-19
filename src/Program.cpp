//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#include "Program.h"
#include "Error.h"
#include <algorithm>
#include <numeric>
#include <fcntl.h>

namespace webzavod
{

Program::Program(const Params & aParams)
	: input(aParams.GetUrl()),
	  output(aParams.GetOutput().empty() ? input.GetFileName() : aParams.GetOutput(), input.GetFileSize()),
	  threadsNumber(input.AcceptRanges() ? aParams.GetThreadsCount() : 1),
	  barrier(threadsNumber, aParams.GetRefresh()),
	  threads(threadsNumber, Thread(input.GetAddress(), &output, &barrier))
{}

class ThreadStartFunc
{
	size_t fileSize, threadsNumber, increment, counter;
public:
	ThreadStartFunc(size_t aFileSize, size_t aThreadsNumber)
		: fileSize(aFileSize), threadsNumber(aThreadsNumber), increment(fileSize/threadsNumber), counter(0) {}
	void operator() (Thread& thread)
	{
		thread.Start(counter*increment, counter!=threadsNumber-1 ? increment: fileSize-counter*increment);
		counter++;
	}
};

class TotalBytesReceived
{
public:
	size_t operator() (size_t total, const Thread& thread)
	{
		return total+thread.GetBytesReceived();
	}
};

class ThreadHasError
{
public:
	bool operator() (const Thread& thread)
	{
		return thread.GetErrorState();
	}
};

void Program::Work()
{
	std::for_each(threads.begin(), threads.end(), ThreadStartFunc(input.GetFileSize(), threadsNumber));
	long bytes(0), old(0);
	std::vector<Thread>::const_iterator thread_with_error;
	timespec start_time, stop_time, transaction_start, transaction_end;
	clock_gettime(CLOCK_REALTIME_COARSE, &transaction_start);
	while (barrier.Count())
	{
		clock_gettime(CLOCK_REALTIME_COARSE, &start_time);
		barrier.TimedWait();
		clock_gettime(CLOCK_REALTIME_COARSE, &stop_time);
		thread_with_error=std::find_if(threads.begin(), threads.end(), ThreadHasError());
		if (thread_with_error!=threads.end())
			throw ThreadWorkerErr();
		bytes=std::accumulate(threads.begin(), threads.end(), 0, TotalBytesReceived());
		std::cout<<"[main thread] Bytes received: "<<bytes<<" of "<<input.GetFileSize();
		std::cout<<" ~ "<<bytes*100/input.GetFileSize()<<"% @ ";
		std::cout<<(bytes-old)*976562/((stop_time.tv_sec-start_time.tv_sec)*1000000000+stop_time.tv_nsec-start_time.tv_nsec);
		std::cout<<"Kb/s"<<"\n";
		old=bytes;
	}
	clock_gettime(CLOCK_REALTIME_COARSE, &transaction_end);
	std::cout<<"[main thread] "<<bytes<<" bytes downloaded successfully at an avg transfer rate @ ";
	std::cout<<bytes*976562/((transaction_end.tv_sec-transaction_start.tv_sec)*1000000000+transaction_end.tv_nsec-transaction_start.tv_nsec);
	std::cout<<"Kb/s; file saved to "<<output.GetFileName()<<"\n";
}

Program::~Program()
{
}

}


