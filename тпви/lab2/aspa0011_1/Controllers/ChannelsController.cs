using aspa0011_1.Models;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics.Tracing;
using System.Reflection;

namespace aspa0011_1.Controllers
{
    [ApiController]
    [Route("api/channels")]
    public class ChannelsController : ControllerBase
    {
        private readonly ChannelService _channelService;
        private ILogger<ChannelService> _logger;
        private int _eventCounter = 0;

        public ChannelsController(ChannelService channelService, ILogger<ChannelService> logger)
        {
            _channelService = channelService;
            _logger = logger;
        }

        [HttpGet("get-all")]
        [ProducesResponseType(200)]
        [ProducesResponseType(204)]
        public ActionResult<List<AspChannel>> GetAll()
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - GetAll",
         ++_eventCounter, DateTime.Now);
            //trace
            var channels = _channelService.GetAllChannels();
            if (channels == null || channels.Count == 0)
                return NoContent();

            return Ok(channels);
        }

        [HttpGet("{id}")]
        [ProducesResponseType(200)]
        [ProducesResponseType(404)]
        public ActionResult<AspChannel> GetById(Guid id)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - GetById {Id}",
         ++_eventCounter, DateTime.Now,id);
            //trace, error
            var channel = _channelService.GetChannelById(id);
            if (channel == null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - GetById, channel == null",
         ++_eventCounter, DateTime.Now);
                return NotFound();
            }
               

            return Ok(channel);
        }

        [HttpPost("create")]
        [ProducesResponseType( 201)]
        [ProducesResponseType(400)]
        [ProducesResponseType(204)]
        public ActionResult<AspChannel> CreateChannel([FromBody] CreateChannelModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - CreateChannel",
         ++_eventCounter, DateTime.Now);
            //trace, error
            var result = _channelService.CreateChannel(model);
            if (result.Channel == null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - CreateChannel, channel == null",
         ++_eventCounter, DateTime.Now);
                return BadRequest(result.Status);
            }
            if (model.State?.ToUpper() == "CLOSED")
            {
                return NoContent();
            }
            return CreatedAtAction(nameof(GetById), new { id = result.Channel.Id }, result.Channel);
        }

        [HttpPost("dequeue-or-peek")]
        [ProducesResponseType(200)]
        [ProducesResponseType(404)]
        public ActionResult<DequeueOrPeekResult> DequeueOrPeek([FromBody] QueueCommandModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - DequeueOrPeek",
         ++_eventCounter, DateTime.Now);
            //trace, error
            var result = _channelService.DequeueOrPeek(model);
            if (result.Data == null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - DequeueOrPeek, data == null",
                ++_eventCounter, DateTime.Now);
                return NotFound(result.Status);
            }
                

            return Ok(result);
        }

        [HttpPost("enqueue")]
        [ProducesResponseType(200)]
        [ProducesResponseType(404)]
        public IActionResult Enqueue([FromBody] QueueEnqueueModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - Enqueue",
        ++_eventCounter, DateTime.Now);
            //trace,error
            var result =  _channelService.Enqueue(model);

            if (result.IsCompletedSuccessfully)
                return Ok(result.Status);

            _logger.LogError("[{EventId}] {Timestamp} - Enqueue, not completed successfully",
        ++_eventCounter, DateTime.Now);
            return NotFound(result.Status);
        }

        [HttpPut("stop-all")]
        [ProducesResponseType(200)]
        [ProducesResponseType(400)]
        public ActionResult<StopAllChannelsResult> StopAllChannels([FromBody] StopAllChannelsModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - StopAllChannels",
        ++_eventCounter, DateTime.Now);
            //trace,error
            var result = _channelService.StopAllChannels(model);
            if (result.Channels == null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - StopAllChannels, no channels to stop",
        ++_eventCounter, DateTime.Now);
                return BadRequest(result.Status);
            }
               

            return Ok(result);
        }

        [HttpPut("stop-one")]
        [ProducesResponseType(200)]
        [ProducesResponseType(404)]
        [ProducesResponseType(400)]
        public ActionResult<StopOneChannelResult> StopOneChannel([FromBody] StopChannelByIdModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - StopOneChannel",
        ++_eventCounter, DateTime.Now);
            //trace,error
            var result = _channelService.StopOneChannel(model);
            if (result.Channel == null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - StopOneChannel, channel == null",
        ++_eventCounter, DateTime.Now);
                if (result.Status.Contains("wrong command"))
                    return BadRequest(result.Status);
                return NotFound(result.Status);
            }
            return Ok(result);
        }

        [HttpPut("resume-all")]
        [ProducesResponseType(200)]
        [ProducesResponseType(400)]
        public ActionResult<ResumeAllChannelsResult> ResumeAll([FromBody] ResumeAllChannelsModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - ResumeALl",
       ++_eventCounter, DateTime.Now);
            //trace, error
            var result = _channelService.ResumeAllChannels(model);
            if (result.Channels == null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - ResumeAll, channels == null",
            ++_eventCounter, DateTime.Now);
                return BadRequest(result.Status);
            }
               

            return Ok(result);
        }

        [HttpPut("resume-one")]
        [ProducesResponseType(200)]
        [ProducesResponseType(404)]
        [ProducesResponseType(400)]
        public ActionResult<ResumeOneChannelResult> ResumeOne([FromBody] ResumeChannelByIdModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - ResumeOne",
++_eventCounter, DateTime.Now);
            //trace,error
            var result = _channelService.ResumeOneChannel(model);
            if (result.Channel == null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - ResumeOne, channel == null",
            ++_eventCounter, DateTime.Now);
                if (result.Status.Contains("command not found"))
                    return BadRequest(result.Status);

                return NotFound(result.Status);
            }
            return Ok(result);
        }

        [HttpDelete("delete-all")]
        [ProducesResponseType(204)]
        [ProducesResponseType(400)]
        public IActionResult DeleteAll([FromBody] DeleteAllChannelsModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - DeleteAll",
        ++_eventCounter, DateTime.Now);
            //trace, error
            var result = _channelService.DeleteAllChannels(model);
            if (result != null)
            {
                _logger.LogError("[{EventId}] {Timestamp} - Delete all, wrong command",
        ++_eventCounter, DateTime.Now);
                return BadRequest(result);
            }
               

            return NoContent();
        }

        [HttpDelete("delete-closed")]
        [ProducesResponseType(typeof(List<AspChannel>), 200)]
        public ActionResult<List<AspChannel>> DeleteClosed([FromBody] DeleteClosedChannelsModel model)
        {
            _logger.LogTrace("[{EventId}] {Timestamp} - DeleteClosed",
        ++_eventCounter, DateTime.Now);
            //trace
            var channels = _channelService.DeleteClosedChannels(model);
            return Ok(channels);
        }
    }
}