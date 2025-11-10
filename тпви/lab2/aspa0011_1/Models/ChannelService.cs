using System.Threading.Channels;
using aspa0011_1.Models;

namespace aspa0011_1.Models//log is stolen idgaf im tired
{
    public class ChannelService
    {
        private ILogger<ChannelService> _logger;
        private int _waitEnqueue;
        private int _eventCounter = 0;
        private List<AspChannel> _aspChannels = new List<AspChannel>();
        private List<Channel<string>> _channels = new List<Channel<string>>();

        public ChannelService(IConfiguration c,ILogger<ChannelService>l) {
            _logger = l;
            _waitEnqueue = c.GetValue<int>("WaitEnqueue",30);

            _logger.LogTrace("[{EventId}] {Timestamp} - ChannelService started",
           ++_eventCounter, DateTime.Now);
        }
        public ChannelService()
        {
           
        }
        

        public List<AspChannel> GetAllChannels()
        {
            return this._aspChannels;
        }

        public AspChannel? GetChannelById(Guid id)
        {
            return this._aspChannels.FirstOrDefault(item => item.Id == id);
        }

        public CreateChannelResult CreateChannel(CreateChannelModel newchannel)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - CreateChannel {Command}",
          ++_eventCounter, DateTime.Now, newchannel.Command);

            if (newchannel.Command == "new")
            {
                var id = Guid.NewGuid();
                var channel = Channel.CreateUnbounded<string>();
                _channels.Add(channel);

                var aspChannel = new AspChannel
                {
                    Id = id,
                    Name = newchannel.Name,
                    Description = newchannel.Description,
                    State = newchannel.State,
                };
                _aspChannels.Add(aspChannel);

                _logger.LogInformation("[{EventId}] {Timestamp} - Channel created: {ChannelId}",
                ++_eventCounter, DateTime.Now, id);

                return new CreateChannelResult
                {
                    Channel = aspChannel,
                    Status = "success"
                };
            }
            else
            {
                return new CreateChannelResult
                {
                    Channel = null,
                    Status = "wrong command, try new"
                };
            }
        }

        public StopAllChannelsResult StopAllChannels(StopAllChannelsModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - StopAllChannels {Command}",
         ++_eventCounter, DateTime.Now, model.Command);
            //debug 
            if (model.Command == "close")
            {
                for (int i = 0; i < _channels.Count; i++)
                {
                    if (_aspChannels[i].State != "Closed")
                    {
                        _channels[i].Writer.Complete();
                        _aspChannels[i].State = "Closed";
                    }
                }
                return new StopAllChannelsResult
                {
                    Channels = _aspChannels,
                    Status = "success",
                    Reason = model.Reason
                };
            }
            return new StopAllChannelsResult
            {
                Channels = null,
                Status = "wrong command, try close",
                Reason = null
            };
        }

        public StopOneChannelResult StopOneChannel(StopChannelByIdModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - StopOneChannel {Command}",
         ++_eventCounter, DateTime.Now, model.Command);
            //debug
            if (model.Command == "close")
            {
                int index = _aspChannels.FindIndex(c => c.Id == model.Id);
                if (index == -1)
                {
                    return new StopOneChannelResult
                    {
                        Channel = null,
                        Status = "channel not found"
                    };
                }
                if (_aspChannels[index].State.ToUpper() == "ACTIVE")
                {
                    _channels[index].Writer.Complete();
                    _aspChannels[index].State = "CLOSED";
                }
                return new StopOneChannelResult
                {
                    Channel = _aspChannels[index],
                    Status = "success"
                };
            }
            return new StopOneChannelResult
            {
                Channel = null,
                Status = "wrong command, try close"
            };
        }

        public ResumeAllChannelsResult ResumeAllChannels(ResumeAllChannelsModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - ResumeAllChannels {Command}",
         ++_eventCounter, DateTime.Now, model.Command);
            //debug
            if (model.Command == "open")
            {
                for (int i = 0; i < _channels.Count; i++)
                {
                    if (_aspChannels[i].State.ToUpper() == "CLOSED")
                    {
                        var newChannel = Channel.CreateUnbounded<string>();
                        _channels[i] = newChannel;
                        _aspChannels[i].State = "ACTIVE";
                    }
                }
                return new ResumeAllChannelsResult
                {
                    Channels = _aspChannels,
                    Status = "success"
                };
            }
            return new ResumeAllChannelsResult
            {
                Channels = null,
                Status = "wrong command, try open"
            };
        }

        public ResumeOneChannelResult ResumeOneChannel(ResumeChannelByIdModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - ResumeOneChannel {Command}",
         ++_eventCounter, DateTime.Now, model.Command);
            //debug
            if (model.Command == "open")
            {
                int index = _aspChannels.FindIndex(_ => _.Id == model.Id);
                if (index == -1)
                {
                    return new ResumeOneChannelResult
                    {
                        Channel = null,
                        Status = "no channel found"
                    };
                }
                if (_aspChannels[index].State.ToUpper() == "CLOSED")
                {
                    _channels[index] = Channel.CreateUnbounded<string>();
                    _aspChannels[index].State = model.State;
                }
                return new ResumeOneChannelResult
                {
                    Channel = _aspChannels[index],
                    Status = "success"
                };
            }
            return new ResumeOneChannelResult
            {
                Channel = null,
                Status = "command not found, try open"
            };
        }

        public string? DeleteAllChannels(DeleteAllChannelsModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - DeleteAllChannels {Command}",
         ++_eventCounter, DateTime.Now, model.Command);
            //debug
            if (model.Command == "del")
            {
                foreach (var channel in _channels)
                {
                    try
                    {
                        channel.Writer.Complete();
                    }
                    catch { }// no need to do something, channel is already closed
                }
                _channels.Clear();
                _aspChannels.Clear();
                return null;
            }
            return "wrong command, try del";// ??????????????
        }

        public List<AspChannel> DeleteClosedChannels(DeleteClosedChannelsModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - DeleteClosedChannels {Command}",
         ++_eventCounter, DateTime.Now, model.Command);
            //debug
            for (int i = _channels.Count - 1; i >= 0; i--)
            {
                if (_aspChannels[i].State == "CLOSED")
                {
                    try
                    {
                        _channels[i].Writer.Complete();
                    }
                    catch { }
                    _channels.RemoveAt(i);
                    _aspChannels.RemoveAt(i);
                }
            }
            return _aspChannels;
        }

        public DequeueOrPeekResult DequeueOrPeek(QueueCommandModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - DequeueOrPeek {Command}",
         ++_eventCounter, DateTime.Now, model.Command);
            //debug
            int index = _aspChannels.FindIndex(_ => _.Id == model.Id);
            if (index == -1)
            {
                return new DequeueOrPeekResult
                {
                    Data = null,
                    Status = "no channel found"
                };
            }
            var channel = _channels[index];
            if (model.Command == "dequeue")
            {
                if (channel.Reader.TryRead(out var value))
                {
                    return new DequeueOrPeekResult
                    {
                        Data = value,
                        Status = "TryRead success"
                    };
                }
                else
                {
                    return new DequeueOrPeekResult
                    {
                        Data = null,
                        Status = "TryRead fail"
                    };
                }
            }
            else if (model.Command == "peek")
            {
                if (channel.Reader.TryPeek(out var value))
                {
                    return new DequeueOrPeekResult
                    {
                        Data = value,
                        Status = "TryPeek success"
                    };
                }
                else
                {
                    return new DequeueOrPeekResult
                    {
                        Data = null,
                        Status = "TryPeek fail"
                    };
                }
            }
            else
            {
                return new DequeueOrPeekResult
                {
                    Data = null,
                    Status = "wrong command, try dequeue or peek"
                };
            }
        }

        public async Task<EnqueueResult> Enqueue(QueueEnqueueModel model)
        {
            _logger.LogDebug("[{EventId}] {Timestamp} - Enqueue attempt for channel: {ChannelId}",
                ++_eventCounter, DateTime.Now, model.Id);

            if (model.Command != "enqueue")
            {
                return new EnqueueResult { Success = false, Status = "wrong command, try enqueue" };
            }

            int index = _aspChannels.FindIndex(_ => _.Id == model.Id);
            if (index == -1)
            {
                return new EnqueueResult { Success = false, Status = "no channel found" };
            }

            var channel = _channels[index];
            var cts = new CancellationTokenSource(TimeSpan.FromSeconds(_waitEnqueue));

            try
            {
                if (await channel.Writer.WaitToWriteAsync(cts.Token))
                {
                    if (channel.Writer.TryWrite(model.Data))
                    {
                       
                        return new EnqueueResult { Success = true, Status = "trywrite success" };
                    }
                }
            }
            catch (OperationCanceledException)
            {
                _logger.LogWarning("[{EventId}] {Timestamp} - WaitEnqueue timeout for channel: {ChannelId}",
                    ++_eventCounter, DateTime.Now, model.Id);
                return new EnqueueResult { Success = false, Status = "timeout: WaitEnqueue expired" };
            }

            return new EnqueueResult { Success = false, Status = "trywrite fail" };
        }
    }
}