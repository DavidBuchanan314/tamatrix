#include "spu.h"

#include <soundio/soundio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

static void write_callback(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) {
	const struct SoundIoChannelLayout *layout = &outstream->layout;
	float float_sample_rate = outstream->sample_rate;
	//float seconds_per_frame = 1.0f / float_sample_rate;
	struct SoundIoChannelArea *areas;
	int frames_left = cpu_offset / (16000000/48000);//frame_count_max;
	int err;
	//fprintf(stderr, "fmax %d\n", frame_count_max);
	
	//if (cpu_offset == 0) return;

	while (frames_left > 0) {
		int frame_count = frames_left;

		if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
			//fprintf(stderr, "%s\n", soundio_strerror(err));
			exit(1);
		}

		if (!frame_count)
			break;

		//printf("fcount %d\n", frame_count);
		int q_ptr = 0;
		//fprintf(stderr, "cpuo %d\n", cpu_offset);
		//fprintf(stderr, "sr %f\n", float_sample_rate);
		for (int frame = 0; frame < frame_count; frame++) {
			while (q_ptr < q_idx && (float) offset_q[q_ptr] / cpu_offset < (float )frame/frames_left) {
				out_state = value_q[q_ptr++];
			}
			for (int channel = 0; channel < layout->channel_count; channel++) {
				unsigned char *ptr = (unsigned char *)(areas[channel].ptr + areas[channel].step * frame);
				*ptr = out_state;
			}
		}

		if ((err = soundio_outstream_end_write(outstream))) {
			fprintf(stderr, "%s\n", soundio_strerror(err));
			exit(1);
		}

		frames_left -= frame_count;
		//fprintf(stderr, "fleft %d\n", frames_left);
	}
	
	q_idx = 0;
	cpu_offset = 0;
}

void q_append(char val) {
	value_q[q_idx] = val + CHAR_MIN;
	offset_q[q_idx] = cpu_offset;
	q_idx++;
	if (q_idx >= QUEUE_LEN) {
		printf("FATAL: SPU event buffer overrun\n");
		exit(-1);
	}
}

int spuInit() {
	int err;
	struct SoundIo *soundio = soundio_create();
	if (!soundio) {
		fprintf(stderr, "out of memory\n");
		return 1;
	}

	if ((err = soundio_connect(soundio))) {
		fprintf(stderr, "error connecting: %s\n", soundio_strerror(err));
		return 1;
	}

	soundio_flush_events(soundio);

	int default_out_device_index = soundio_default_output_device_index(soundio);
	if (default_out_device_index < 0) {
		fprintf(stderr, "no output device found\n");
		return 1;
	}

	struct SoundIoDevice *device = soundio_get_output_device(soundio, default_out_device_index);
	if (!device) {
		fprintf(stderr, "out of memory\n");
		return 1;
	}

	fprintf(stderr, "Output device: %s\n", device->name);

	struct SoundIoOutStream *outstream = soundio_outstream_create(device);
	if (!outstream) {
		fprintf(stderr, "out of memory\n");
		return 1;
	}
	outstream->format = SoundIoFormatU8;
	outstream->write_callback = write_callback;
	 outstream->software_latency = 0.0;

	if ((err = soundio_outstream_open(outstream))) {
		fprintf(stderr, "unable to open device: %s", soundio_strerror(err));
		return 1;
	}

	if (outstream->layout_error)
		fprintf(stderr, "unable to set channel layout: %s\n", soundio_strerror(outstream->layout_error));

	if ((err = soundio_outstream_start(outstream))) {
		fprintf(stderr, "unable to start device: %s\n", soundio_strerror(err));
		return 1;
	}

	return 0;
}
