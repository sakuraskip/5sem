namespace aspa0011_1.Models
{
    public class CreateChannelResult
    {
        public AspChannel? Channel { get; set; }
        public string Status { get; set; } = null!;
    }

    public class StopAllChannelsResult
    {
        public List<AspChannel>? Channels { get; set; }
        public string Status { get; set; } = null!;
        public string? Reason { get; set; }
    }

    public class StopOneChannelResult
    {
        public AspChannel? Channel { get; set; }
        public string Status { get; set; } = null!;
    }

    public class ResumeAllChannelsResult
    {
        public List<AspChannel>? Channels { get; set; }
        public string Status { get; set; } = null!;
    }

    public class ResumeOneChannelResult
    {
        public AspChannel? Channel { get; set; }
        public string Status { get; set; } = null!;
    }

    public class DequeueOrPeekResult
    {
        public string? Data { get; set; }
        public string Status { get; set; } = null!;
    }

    public class EnqueueResult
    {
        public bool Success { get; set; }
        public string Status { get; set; } = null!;
    }
}
