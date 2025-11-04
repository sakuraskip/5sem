namespace aspa0011_1.Models
{
    public class AspChannel
    {
        public Guid Id { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string State { get; set; }

    }
    public class CreateChannelModel
    {
        public string Command { get; set; }
        public string Name { get; set; }
        public string State { get; set; }
        public string Description { get; set; }
    }
    public class StopAllChannelsModel
    { 
        public string Command { get; set; }
        public string Reason { get; set; }
    }
    public class StopChannelByIdModel
    {
        public string Command { get; set; }
        public Guid Id { get; set; }
        public string Reason { get; set; }
    }
    public class ResumeAllChannelsModel
    {
        public string Command { get; set; }
    }
    public class ResumeChannelByIdModel
    {
        public string Command { get; set; }
        public Guid Id { get; set; }
        public string State { get; set; }
    }
    public class DeleteAllChannelsModel
    {
        public string Command { get; set; }
    }
    public class DeleteClosedChannelsModel
    {
        public string Command
        {
            get; set;
        }
        public string State { get; set; }
    }
    public class QueueCommandModel
    {
        public string Command { get; set; }  
        public Guid Id { get; set; }         
    }
    public class QueueEnqueueModel
    {
        public string Command { get; set; }  
        public Guid Id { get; set; }          
        public string Data { get; set; }      
    }

    public class QueueErrorModel
    {
        public Guid Id { get; set; }
        public string Error { get; set; }
    }


}
