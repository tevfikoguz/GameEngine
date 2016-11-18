// Copyright 2016-2017 ?????????????. All Rights Reserved.
#pragma once
#include <Parallelizing\Job.h>
#include <Threading\Thread.h>
#include <Common\ThreadSafeQueue.h>

#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

namespace Engine
{
	namespace Threading
	{
		class Thread;
		class Fiber;
	}

	using namespace Threading;

	namespace Parallelizing
	{
		class JobManager
		{
		public:
			JobManager(void);

			void Add(JobDescription *Description);

			static JobManager &GetInstance(void)
			{
				if (instance == nullptr)
					instance = new JobManager();

				return *instance;
			}

		private:
			static void ThreadWorker(void *Arguments);
			static void MainFiberWorker(void *Arguments);
			static void JobFiberWorker(void *Arguments);

		private:
			uint8 m_ThreadCount;
			Thread *m_Threads;
			Fiber *m_Fibers;
			ThreadSafeQueue<Job*> m_Jobs;
			static JobManager *instance;
		};

		JobDescription *CreateJobDescription(JobDescription::Procedure Procedure, void *Arguments);
		JobDescription *AddJob(JobDescription::Procedure Procedure, void *Arguments);

#ifndef DECLARE_JOB
#define DECLARE_JOB(Class, Name) \
	struct __Job_Arguments_##Name \
	{ \
	public: \
		Class *Instance; \
	}; \
	static void __Job_Entry_Point_##Name(void *Arguments) \
	{ \
		__Job_Arguments_##Name *arguments = reinterpret_cast<__Job_Arguments_##Name*>(Arguments); \
		arguments->Instance->Name(); \
	} \
	static JobDescription *__Job_Runner_##Name(Class *Instance) \
	{ \
		__Job_Arguments_##Name *__Job_Arguments_##Name##_Ptr = new __Job_Arguments_##Name; \
		__Job_Arguments_##Name##_Ptr->Instance = Instance; \
		return AddJob(__Job_Entry_Point_##Name, __Job_Arguments_##Name##_Ptr); \
	} \
	void Name()
#endif

#ifndef RUN_JOB
#define RUN_JOB(Name) __Job_Runner_##Name(this)
#endif
	}
}

#endif